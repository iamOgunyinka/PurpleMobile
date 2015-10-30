import bb.cascades 1.2
import bb.data 1.0
import purple.downloadManager 1.0
import bb.system 1.2

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
        topPadding: 20
        signal finished();
        
        attachedObjects: [
            DataSource {
                id: dataSource
                source: "asset:///download_queue.json"
                onDataLoaded: {
                    for( var i = 0; i != data.length; ++i ){
                        arrayDataModel.append( data[i] );
                    }
                }
            },
            CppDownloadManager {
                id: myDownloadManager
            },
            SystemToast {
                id: systemToast
            }
        ]
        onCreationCompleted: {
            dataSource.load()
        }
        
        ListView {
            dataModel: ArrayDataModel {
                id: arrayDataModel
            }
            
            listItemComponents: [
                ListItemComponent {
                    id: listItem
                    type: ""
                    CustomListItem {
                        contextActions: [
                            ActionSet {
                                DeleteActionItem {
                                    onTriggered: {
                                        var myView = listItem.ListItem.view
                                        var dataModel = myView.dataModel
                                        var indexPath = myView.selected()
                                        dataModel.removeItem( indexPath )
                                    }
                                }
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
        downloadStatus.text = "Downloaded " + actualReceived + " of " + actualTotal
    
    }
    
    //TODO
    function status( url, title, status, data )
    {
        if( title == "Download started" ){
            
        } else if( title == "Error" ) {
            
        } else if( title == "Cancel" ) {
            
        } else { //Complete Download
            
        }
    }
    
    function startNewDownload( url )
    {
        myDownloadManager.startDownload( url );
    }
    
    function showCompletedDownloads()
    {
        //
    }
    
    function showStoppedDownloads()
    {
        //
    }
}