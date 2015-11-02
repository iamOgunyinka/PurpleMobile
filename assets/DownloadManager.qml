import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2
import purple.downloadManager 1.0
import purple.model 1.0

Container {
    id: downloadPageTab
    Container {
        id: rootContainer
        topPadding: 20
        signal finished();
        
        attachedObjects: [
            CppDownloadManager {
                id: myDownloadManager
            },
            SystemToast {
                id: systemToast
            }
        ]
        
        ListView {
            dataModel: CppDataModel {
                id: dataModel
                source: "download_queue.json"
            }
            
            listItemComponents: [
                ListItemComponent {
                    id: listItem
                    type: "all"
                    CustomListItem {
                        contextActions: [
                            ActionSet {
                                actions: [
                                    DeleteActionItem {
                                        onTriggered: {
                                            var myView = listItem.ListItem.view
                                            var dataModel = myView.dataModel
                                            var indexPath = myView.selected()
                                            dataModel.removeItem( indexPath )
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
                                                myDownloadManager.pauseDownload( m_url )
                                                title = "Resume"
                                            } else {
                                                myDownloadManager.resumeDownload( m_url )
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
                                            
                                            myDownloadManager.startDownload( m_url )
                                        }
                                    }
                                ]
                            }
                        ]
                        Container {
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            ImageView {
                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 1/6
                                }
                                imageSource: ListItemData.thumbnail
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
                                    value: 5
                                }
                                Label {
                                    id: downloadStatus
                                    textStyle {
                                        fontStyle: FontStyle.Italic   
                                        fontSize: FontSize.XSmall
                                        color: Color.Blue
                                    }
                                }
                                Label {
                                    id: urlKeeper
                                    visible: false
                                    text: ListItemData.url
                                }
                            }
                        }
                    }
                }
            ]
        }
    }
    onCreationCompleted: {
        myDownloadManager.status.connect( downloadPageTab.status )
        myDownloadManager.progress.connect( downloadPageTab.progress )
        myDownloadManager.finished.connect( downloadPageTab.finished )
        myDownloadManager.newDownloadAdded.connect( newDownloadAdded )
    }
    
    function newDownloadAdded()
    {
        dataModel.setSource( "download_queue.json" );
    }
    
    function finished( m_url, destination )
    {
        systemToast.body = "Download Completed"
        systemToast.exec();
    }
    
    function progress( m_url, actualReceived, actualTotal, percent, speed, unit )
    {
        progressIndicator.value = percent
        var downloadProgressText = "Downloaded " + actualReceived + " of " + actualTotal
        console.log( "Downloaded ", actualReceived, " with total speed of ", speed, unit )
//        downloadStatus.text = downloadProgressText
    }
    
    //TODO
    function status( m_url, title, status, data )
    {
        if( title == "Download started" ){
            systemToast.body = "Download started";
            systemToast.exec();
        } else if( title == "Error" ) {
            console.log( "Error reported ", status )
        } else if( title == "Cancel" ) {
            systemToast.body = "Download already completed";
            systemToast.exec()
        } else { //Complete Download
            console.log( "Download Completed" )
        }
    }
    
    function startNewDownload( url )
    {
        myDownloadManager.startDownload( url );
    }
    
    // TODO
    function showCompletedDownloads()
    {  
    }
    
    function showStoppedDownloads()
    {
        //
    }
}