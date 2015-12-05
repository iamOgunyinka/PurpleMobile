import bb.cascades 1.0
import bb.system 1.0
import bb.data 1.0
import purple.ytde 1.0
import purple.image 1.0

TabbedPane {
    id: root
    showTabsOnActionBar: true
    attachedObjects: [
        Container {
            id: videoContainer
            ImageView {
                id: imgVideoThumbnail
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                scalingMethod: ScalingMethod.AspectFit
            }
            Label {
                id: lblVideoDuration
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }
            Label {
                id: lblTitle
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }
        },
        SystemDialog {
            id: errorDialog
            title: "Error"
            body: ""
        },
        SystemListDialog {
            id: list_of_video_formats
            
            onFinished: {
                if( value == SystemUiResult.ConfirmButtonSelection ){
                    ytDownloadExtractor.downloadVideoWithER( 
                        list_of_video_formats.selectedIndices[0]
                        )
                }
            }
        },
        SystemProgressDialog {
            id: progress
            title: "Retrieving video information"
            onFinished: {
                if( value == SystemUiResult.CancelButtonSelection ){
                    // TODO
                }
            }
        },
        CppYTDownloadExtractor {
            id: ytDownloadExtractor
        },
        CppImageFetcher {
            id: imageFetcher
        }
        
    ]
    Tab {
        id: homeTab
        title: "Home"
        imageSource: "asset:///ad_search.png"
        
        Page {
            titleBar: TitleBar {
                id: homepageTitleBar
                title: "Purple"
                acceptAction: ActionItem {
                    title: "Close"
                    onTriggered: {
                        Application.minimize()
                    }
                }
            }
            id: homepageTab
            Container {
                topPadding: 20
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    leftPadding: 20
                    rightPadding: 20
                    TextField {
                        id: txtUrl
                        hintText: "Enter URL"
                        input {
                            submitKey: SubmitKey.Submit
                            onSubmitted: {
                                ytDownloadExtractor.getDownloadInfo( txtUrl.text )
                                progress.show()
                            }
                        }
                    }
                    
                    Button {
                        id: searchButton
                        imageSource: "asset:///download.png"
                        maxWidth: 1
                        
                        onClicked: {
                            ytDownloadExtractor.getDownloadInfo( txtUrl.text )
                        }
                    }
                }
                Container {
                    leftPadding: 20
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center

                    controls: videoContainer
                }
            }
            
            function onStreamsAvailable( streams )
            {
                lblVideoDuration = ytDownloadExtractor.time
                lblTitle = ytDownloadExtractor.title
                imageFetcher.fetchImage( ytDownloadExtractor.thumbnail )
                
                for( var i = 0; i < streams.length; ++i )
                {
                    list_of_video_formats.appendItem( streams[i] )
                }
            }
            
            function onUrlExtracted( videoUrl )
            {
                downloadManager.startNewDownload( videoUrl )
            }
            
            function onImageFetched( location )
            {
                imgVideoThumbnail.imageSource = location
            }
            
            onCreationCompleted: {
                ytDownloadExtractor.finished.connect( homepageTab.onStreamsAvailable )
                ytDownloadExtractor.url.connect( homepageTab.onUrlExtracted )
                imageFetcher.imageFetched.connect( homepageTab.onImageFetched )
            }
        }
    }
    
    Tab {
        id: downloadsTab
        title: "Downloads"
        imageSource: "asset:///download.png"
        content: Page {
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
                    switch ( value ){
                        case 0: break;
                        case 1: {
                                downloadManager.showCompletedDownloads(); 
                                break;
                        }
                    case 2: {
                            downloadManager.showStoppedDownloads();
                            break;
                    }
                    }
                }
                id: downloadsTitleBar
                title: "Downloads"
            }
            DownloadManager {
                id: downloadManager
            }
        }
    }
    
    Tab {
        title: "Help me"
        imageSource: "asset:///property.png"
        Page {
            // TODO
        }
    }
}