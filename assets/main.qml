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
            body: "Please wait..."
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
                title: "Purple Mobile"
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
                SegmentedControl {
                    id: selectedDownloadOption
                    options: [
                        Option {
                            text: "Direct Download"
                            value: 1
                        },
                        Option {
                            text: "Youtube Videos"
                            value: 2
                        }
                    ]
                    onCreationCompleted: {
                        selectedDownloadOption.setSelectedIndex( 0 )
                    }
                }
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
                                homepageTab.download( txtUrl.text )
                            }
                        }
                    }
                    
                    Button {
                        id: searchButton
                        imageSource: "asset:///download.png"
                        maxWidth: 1
                        
                        onClicked: {
                            homepageTab.download ( txtUrl.text )
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
            
            function download( text )
            {
                if( selectedDownloadOption.selectedValue == 1 ){
                    downloadManager.startNewDownload( text )
                } else {
                    ytDownloadExtractor.getDownloadInfo( text )
                    progress.show()
                }
            }
            
            function onStreamsAvailable( streams )
            {
                lblVideoDuration = ytDownloadExtractor.time
                lblTitle = ytDownloadExtractor.title
                imageFetcher.fetchImage( ytDownloadExtractor.thumbnail )
                
                list_of_video_formats.clearList()
                for( var i = 0; i < streams.length; ++i )
                {
                    list_of_video_formats.appendItem( streams[i] )
                }
                progress.cancel()
            }
            
            function onError( error )
            {
                console.log( error )
                progress.cancel()
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
                ytDownloadExtractor.error.connect( homepageTab.onError )
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
            titleBar: TitleBar {
                title: "Help"
            }
            Container {
                layout: StackLayout {}
                topPadding: 20
                leftPadding: 20
                controls: Help {
                    
                }
            }
        }
    }
}