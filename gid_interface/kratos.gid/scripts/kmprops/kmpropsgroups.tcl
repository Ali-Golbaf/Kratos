#####################################################################################
#
#  NAME: kmpropsgroups.tcl
#
#  PURPOSE: Manage the group options in the kratos main model window 
#
#  QUANTECH ATZ-DEVELOPMENT DEPARTMENT
#
#  AUTHORS : G. Socorro and L. Calvo
#
#  CREATED AT: 29/03/2012
#
#  HISTORY:
# 
#   0.7- 18/06/13- G. Socorro, delete the use of the proc kipt::NewGiDGroups (delete the call to the compass groups => Cond_Groups)
#   0.6- 12/02/12-G. Socorro, modify the link to the GiD group window to use ::WinUtils::OpenGiDGroupTab
#   0.5- 10/10/12-J. Garate, Adaptation for New GiD Groups, Autogroup Frame Bug Corrected when assigning entities
#   0.4- 01/10/12-J. Garate, Deleting group function corrected
#   0.3- 26/04/12-G. Socorro, change GiD_Groups by Cond_Groups
#   0.2- 02/04/12-G. Socorro, correct a bug with the combobox path (update the autoNewGroup proc)
#   0.1- 29/03/12-G. Socorro, create a base source code from the kmprops.tcl script
#
######################################################################################
#                      Procedures that belong to this file
###############################################################################
#         Name                      |        Functionality
#------------------------------------------------------------------------------
# 1.            | 


proc ::KMProps::changeGroups { entityList f {fullname ""} } {
    
        set valores [GiD_Groups list]
    $f configure -values $valores
    
    if { !($::KMProps::selGroup in $valores) } {
	
	if {[string range $::KMProps::selGroup 0 8] == "AutoGroup"} {
	    WarnWin [= "The new group '%s' has not any usefull entity assigned." $::KMProps::selGroup]
	}
	set ::KMProps::selGroup ""
    } 
}

proc ::KMProps::cmbChangeCheckGroups { f } {

    if { [winfo exists $f.cGroups] } {
            set Groups [GiD_Groups list]
        
        if { [llength $Groups] } {
            
            $f.cGroups configure -values $Groups
            if { $::KMProps::selGroup ni $Groups} {
            set ::KMProps::selGroup [lindex $Groups 0]
            }
        } else {
            $f.cGroups configure -values {}
            set ::KMProps::selGroup ""
        }
    }
}

proc ::KMProps::setNoActiveGroups { node } {
    
    if {[$node getAttribute class ""] == "Groups" } {
	
	foreach nod [$node childNodes] {
	    if {[$nod getAttribute class ""] == "Group" } {
		$nod setAttribute active 0
	    }
	}
    }
}

proc ::KMProps::getGroups { entityList {fullname ""}} {
    
    global KPriv
    
    # Switch state
    if { $entityList == "" } {
	set PState [GiD_Info Project ViewMode]
	if {($PState == "GEOMETRYUSE")} {
	    set entityList {point line surface volume}
	} else {
	    set entityList {nodes elements}
	}
    }
    
    set grupos {}
        set grw "GiD_Groups"
    foreach groupId [$grw list] {
        foreach entity $entityList {
            if { [::KEGroups::getGroupGiDEntities $groupId $entity "hasEntities"] } {
            if { !( $groupId in $grupos) } {
                lappend grupos $groupId
            }
            }
        }
    }
    
    
    if {$fullname != ""} {
	
        #Eliminamos de la lista los grupos ya asignados a esta propiedad
        set assignedGroups [::xmlutils::setXmlContainerIds $fullname]
        foreach g $assignedGroups {
            if { $g != $::KMProps::selGroup } {
            set grupos [::KEGroups::listReplace $grupos $g]
            }
        }
    }
    return $grupos
}

proc ::KMProps::autoNewGroup { id fpath } {
    variable selGroup
    global KPriv
    
    set GroupId [::KEGroups::GetAutomaticGroupName "Auto"]
    
    # Assign the selected group identifier
    set selGroup $GroupId 
    
    # Create the new group
        GiD_Groups create $GroupId
    # Selection the entities to be assigned
    ::KEGroups::SelectionAssign $::KMProps::selectedEntity $GroupId $fpath
    
    # Ponemos el foco en la ventana de propiedades
    #focus $winpath
    
}

proc ::KMProps::acceptGroups { T idTemplate fullname item listT entityList fGroups} {
    variable selGroup
    variable NbPropsPath

    set grupo $selGroup
    
    if { $grupo == "" } {
        WarnWin [= "You have to choose one group\n (you can create a new one pushing the button on the right)"]
    } else {
        
        #Primero comprobamos q el grupo aun no exista
        set id [::xmlutils::setXml "$fullname//c.$grupo" id]
        
        if { $id != "" } {
            
            WarnWin [= "This group it is already assigned to this condition."]
        } else {
            
            #Validamos que haya alguna propiedad seleccionada
            if {[info exists ::KMProps::cmbProperty]} {
            if {$::KMProps::cmbProperty == "" } {
                WarnWin [= "You must define a Property before!"]
                return ""
            }
            }
            
            #Comprobamos que el grupo no sea un AutoGroup sin entidades
            ::KMProps::changeGroups $entityList $fGroups
            if { $::KMProps::selGroup == "" } {
            return ""
            }
            
            set template [::KMProps::copyTemplate ${idTemplate} $fullname "$grupo" "Group"]
            #Validamos que haya alg�n combo de "properties" en el template
            if {[string match "*GCV=\"Properties*" $template]} {
            
            #Miramos si hay alguna propiedad dada de alta (si la hay,obligatoriamente estar� seleccionada)
            set props [::xmlutils::setXmlContainerIds "[::KMProps::getApplication $fullname]//c.Properties"]
            if { [llength $props] < 1 } {
                WarnWin [= "You must define a Property before."]
                #::KMProps::deleteProps $T $itemSel $newPropertyName
                return  ""
            }
            }
            
            
            #
            # Ahora debemos actualizar todos los valores en el xml
            #
            set fBottom ${NbPropsPath}.fBottom
            
            if {[llength $listT] >= 1 } {
            
            #Lista de listas con formato {idContainer idItem1 idItem2...}
            foreach listContainer $listT {
                
                #Si tiene como m�nimo el container y un item entramos
                if {[llength $listContainer] >= 2} {
                    
                    set idContainer [lindex $listContainer 0]
                    
                    #set id [::KMProps::getPropTemplate $idTemplate id $idContainer]
                    
                    #Recorremos los items
                    for {set i 1} { $i < [llength $listContainer] } {incr i} {
                        
                        set id [lindex $listContainer $i]
                        
                        #Cuando tengamos nodos ocultos o incompatibles la variable no existir�
                        if {[info exists ::KMProps::cmb$id]} {
                            
                            set fullNombre "$fullname//c.$grupo//c.$idContainer//i.$id"
                            
                            set f "${fBottom}.nb.f${idContainer}.cmb${id}"
                            
                            if { [winfo exists $f] } {
                                
                                if { [$f cget -state] == "readonly" } {
                                    
                                    set value [::xmlutils::getComboDv $f $fullNombre]
                                } else {
                                    
                                    set value [set ::KMProps::cmb$id]                                                                          
                                }
                                
                                if {$id == "Vx" || $id == "Vy" || $id == "Vz"} {
                                    
                                    set activeId "A[string range $id 1 1]"
                                    set fullActive "$fullname//c.$grupo//c.Activation//i.$activeId"
                                    set active [::xmlutils::setXml $fullActive dv "read"]
                                    
                                    if { $active == 0 } {
                                        ::xmlutils::setXml $fullNombre state "write" "disabled"
                                    } else {
                                        ::xmlutils::setXml $fullNombre state "write" "normal"
                                    }
                                }
                                #Comprobamos si el combo tiene una funci�n asignada
                                set function [::xmlutils::setXml $fullNombre function]
                                if { $function != "" && [$f cget -state] == "disabled"} {
                                    
                                    ::xmlutils::setXml $fullNombre function "write" 1
                                    ::xmlutils::setXml $fullNombre state "write" "disabled"
                                } 
                                
                                #Guarda el nuevo valor en el xml
                                ::xmlutils::setXml $fullNombre dv "write" $value
                            }
                        }
                    }
                }
            }
            }

            #Destruimos el frame inferior
            ::KMProps::DestroyBottomFrame
            
            ::KMProps::RefreshTree $T
            
            $T selection add $item
            $T item expand $item
		# Open the new group tab
		::WinUtils::OpenGiDGroupTab
        }
    }
}

#
# Borrar el grupo de la condici�n preguntando si lo queremos borrar completamente
#
proc ::KMProps::deleteGroupCondition { T item } {
    #msg "$T $item"
    set fullname [DecodeName [$T item tag names $item]]
    set GroupId [$T item text $item 0]
    
    set aviso "Removing group $GroupId. Please, choose the properly option:\n\n\n Yes: Desassign this group (recomended)\n\n No: Complete group removing (with all its descendants).\n\n Cancel: Keep the group assigned."
    set answer [::WinUtils::confirmBox "." "$aviso" "yesnocancel"]
    if { $answer == "yes" } {
	
        #Desasigna de la gemoetr�a el item seleccionado
        #::KEGroups::UnAssignCondition $GroupId
        
        #Elimina el grupo del xml
        ::xmlutils::unsetXml [DecodeName [$T item tag names $item]]
        
        #Elimina el grupo del �rbol
        ::KMProps::deleteItem $T $item
        
    } elseif {$answer == "no" } {
    
        ::KEGroups::BorraGrupo [lindex [split [DecodeName [$T item tag names $item]] "."] end]
        ::xmlutils::unsetXml [DecodeName [$T item tag names $item]]
        ::KMProps::deleteItem $T $item
        
        
    } else {
    }
    
}

#
# Busca en el arbol de propiedades, todos los grupos que se llamen $GroupId
#
proc ::KMProps::findGroups { GroupId } {
    
    global KPriv
    
    set ElemList { }
    set bool 0
    #Primero hay que asegurarse de que exista el �rbol
    if { [winfo exists $::KMProps::WinPath] } {
	
	#Si est� el frameBottom activo nos lo cargamos
	::KMProps::DestroyBottomFrame
	
	set T $::KMProps::TreePropsPath
	# Recorremos tooodo el �rbol
	set grupos [$T item descendants "root"]
	foreach item $grupos {
	    #Comprobamos q el item aun exista
	    if { [$T item id $item] != "" } {
		set itemId [$T item text $item 0]
		#msg $GroupId
		#msg $itemId
		if { $itemId == $GroupId } {
		    set fullname [DecodeName [$T item tag names $item]]     
		    lappend ElemList $fullname $T $item
		    set bool 1
		    #::KMProps::deleteGroupCondition $T $item
		}
	    }
	}
	#Cierra la ventana de propiedades y la vuelve a cargar para q se renombren
	# los items autom�ticamente
	#::KMProps::RefreshTree $T
	
    } else {
	
	#Si no est� abierta la ventana, deberemos recorrer el xml a mano
	set nodes [$KPriv(xml) getElementsByTagName "Container"]
	
	foreach node $nodes {
	    
	    catch {
		#Si encuentra el grupo intenta borrar el nodo y su descendencia
		if { [$node getAttribute id ""] == "$GroupId" } {
		    if { [$node getAttribute class ""] == "Group" } {
		        lappend ElemList $node
		        set bool 1
		    }
		}
	    }
	}
	
    }
    if { $bool == 0 } {
	set ElemList 0
    }
    return $ElemList
}

