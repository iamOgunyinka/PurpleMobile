import bb.cascades 1.2
import bb.data 1.0
import bb.network 1.0

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
                        showCompletedDownloads(); 
                        break;
                }
            case 2:{
                    showStoppedDownloads();
                    break;
            }
            }
        }
        id: downloadsTitleBar
        title: "Downloads"
    }
    
    Container {
        ListView {
            dataModel: XmlDataModel{
                source: "asset:///download_queue.xml"
            }
            listItemComponents: [
                ListItemComponent {
                    type: "category"
                    Header {
                        title: ListItemData.value
                    }
                },
                ListItemComponent {
                    type: "download"
                    CustomListItem {
                        Container {
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            ImageView {
                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 1/6
                                }
                                imageSource: ListItemData.thumbnails
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
                                    fromValue: 1
                                    toValue: 100
                                    value: 5
                                    onValueChanged: {
                                        if( Math.round(value) == 100 ){
                                            visible = false
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            ]
        }
        // free functions
        
        function showCompletedDownloads()
        {
            //
        }
        
        function showStoppedDownloads()
        {
            //
        }
    }
}