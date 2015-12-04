import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2
import purple.model 1.0

Container {
    id: downloadPageTab
    Container {
        id: rootContainer
        topPadding: 20
        signal finished();
        
        attachedObjects: [
            CppDataModel {
                id: myDataManager
            },
            SystemToast {
                id: systemToast
            }
        ]
        
        ListView {
            id: listView
            
            listItemComponents: [
                ListItemComponent {
                    id: listItem
                    type: ""
                    CustomListItem {
                        Container {
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            ImageView {
                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 1/6
                                }
                                imageSource: "asset:///downloads_icon.png"
                                scalingMethod: ScalingMethod.AspectFit
                            }
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.TopToBottom
                                }
                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 5/6
                                }
                                Label {
                                    text: ListItemData.title
                                }
                                ProgressIndicator {
                                    id: progressIndicator
                                    fromValue: 1
                                    toValue: 100
                                    value: ListItemData.percentage
                                }
                                Label {
                                    id: downloadStatus
                                    text: ListItemData.status
                                    textStyle {
                                        fontStyle: FontStyle.Italic
                                        fontSize: FontSize.XSmall
                                        color: Color.Blue
                                    }
                                }
                            }
                        }
                        contextActions: [
                            ActionSet {
                                actions: [
                                    DeleteActionItem {
                                        onTriggered: {
                                            var dataModel_ = listView.dataModel
                                            var indexPath = listView.selected()
                                            myDataManager.removeItem( indexPath )
                                        }
                                    },
                                    ActionItem {
                                        title: "Pause"
                                        onTriggered: {
                                            var dataModel = listItem.ListItem.view.dataModel
                                            var indexPath = listItem.ListItem.view.selected()
                                            var selectedDownload = dataModel.data( indexPath )
                                            var m_url = selectedDownload["url"];
                                            
                                            if( title == "Pause" ){                                            
                                                myDataManager.pauseDownload( m_url )
                                                title = "Resume"
                                            } else {
                                                myDataManager.resumeDownload( m_url )
                                                title = "Pause"
                                            }
                                        }
                                    },
                                    ActionItem {
                                        title: "Stop"
                                        onTriggered: {
                                            var dataModel = listItem.ListItem.view.dataModel
                                            var indexPath = listItem.ListItem.view.selected()
                                            var selectedDownload = dataModel.data( indexPath )
                                            var m_url = selectedDownload["url"];
                                            
                                            myDataManager.startDownload( m_url )
                                        }
                                    }
                                ]
                            }
                        ]
                    }
                }
            ]
        }
    }
    onCreationCompleted: {
        myDataManager.status.connect( downloadPageTab.status )
        myDataManager.error.connect( downloadPageTab.error )
    }
    
    function status( m_url, destination )
    {
        systemToast.body = "Download Completed"
        systemToast.exec();
    }
    
    //TODO
    function error( message )
    {
        console.log( message )
    }
    
    function startNewDownload( url )
    {
        myDataManager.startDownload( url );
    }
    
    // TODO
    function showCompletedDownloads()
    {
    }
    
    //TODO
    function showStoppedDownloads()
    {
    }
}
