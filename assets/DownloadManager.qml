import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2
import purple.downloadManager 1.0
import purple.model 1.0

Page {
    id: downloadPageTab
    titleBar: TitleBar {
        kind: TitleBarKind.Segmented
        options: [
            Option {
                text: "All"
                value: 0
            },
            Option {
                text: "Completed"
                value: 1
            },
            Option {
                text: "Stopped"
                value: 2
            }
        ]
        onSelectedOptionChanged: {
            var value = selectedOption.value
            switch ( value )
            {
                case 0: break;
                case 1:{
                        downloadPageTab.showCompletedDownloads(); 
                        break;
                }
            case 2:{
                    downloadPageTab.showStoppedDownloads();
                    break;
            }
            }
        }
        id: downloadsTitleBar
        title: "Downloads"
    }
    
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
    }
    
    function finished(url, destination)
    {
        systemToast.body = "Download Completed"
    }
    
    function progress( url, actualReceived, actualTotal, percent, speed, unit )
    {
        progressIndicator.value = percent
        var downloadProgressText = "Downloaded " + actualReceived + " of " + actualTotal
        console.log( downloadProgressText )
        downloadStatus.text = downloadProgressText
    }
    
    //TODO
    function status( url, title, status, data )
    {
        if( title == "Download started" ){
            console.log( "Download Started" );
        } else if( title == "Error" ) {
        
        } else if( title == "Cancel" ) {
        
        } else { //Complete Download
        
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