import bb.cascades 1.0

Container {
    property string thumbnails
    property string appTheme
    property string fileExist
    property string safeSearch
    property int maxResults
    
    signal thumbnailsQualityChanged( string value )
    signal appThemeChanged( string value )
    signal maxResultChanged( int value )
    signal safeSearchChanged( string value )
    signal fileExistChanged( string value )
    
    topPadding: 30
    leftPadding: 30
    rightPadding: 30
    
    id: root
    onCreationCompleted: {
        appThemeOption.setSelectedIndex( appTheme == "Light" ? 0 : 1);
        
        if( safeSearch == "None") safeSearchOption.setSelectedIndex(0)
        else if ( safeSearch == "Moderate") safeSearchOption.setSelectedIndex(1)
        else safeSearchOption.setSelectedIndex(2)
        
        sliderMaxResult.setValue( maxResults )

        var fileExistPolicyIndex = ( fileExist == "Cancel" ? 0 : ( fileExist == "Overwrite" ? 1 : 2 ) )
        fileExistsOption.setSelectedIndex( fileExistPolicyIndex )
        
        var thumbnailsQualityOptionIndex = ( thumbnails == "Default" ? 0 : ( thumbnails == "Medium" ? 1 : 2 ) )
        thumbnailsQualityOption.setSelectedIndex( thumbnailsQualityOptionIndex )
    }
    
    DropDown {
        id: appThemeOption
        title: "Theme"
        options: [
            Option {
                text: "Light"
            },
            Option {
                text: "Dark"
            }
        ]
        onSelectedOptionChanged: {
            root.appThemeChanged( selectedOption.text )
        }
    }
    Divider {}
    
    DropDown {
        id: safeSearchOption
        title: "Safe Search"
        options: [
            Option {
                text: "None"
                value: 0
            },
            Option {
                text: "Moderate"
                value: 1
            },
            Option {
                text: "Strict"
                value: 2
            }
        ]
        onSelectedOptionChanged: {
            root.safeSearchChanged( selectedOption.text )
        }
    }
    DropDown {
        id: thumbnailsQualityOption            
        title: "Thumbnails Quality"
        onSelectedOptionChanged: {
            root.thumbnailsQualityChanged( selectedOption.text )
        }
        options: [
            Option {
                text: "Default"
            },
            Option {
                text: "Medium"
            },
            Option {
                text: "High"
            }
        ]
    }
    Divider {}
    Container {
        Label {
            id: maximumResultsLabel
            text: "Results per search"
        }
        Slider {
            id: sliderMaxResult
            fromValue: 5
            toValue: 50
            onValueChanged: {
                var val = Math.round( value )
                maximumResultsLabel.text = "Results per search: " + val
                root.maxResultChanged( val )
            }
        }
    }
    Divider {}
    
    Container {
        Label {
            text: "If file already exist"
        }
        DropDown {
            id: fileExistsOption
            options: [
                Option {
                    text: "Cancel"
                },
                Option {
                    text: "Overwrite"
                },
                Option {
                    text: "Rename"
                }
            ]
            onSelectedOptionChanged: {
                root.fileExistChanged( selectedOption.text )                
            }
        }
    }
}