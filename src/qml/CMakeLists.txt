set(ktouch_qml_files
    main.qml

    HomeScreen.qml
    HomeScreenAccordion.qml
    HomeScreenBox.qml
    ProfileSelector.qml
    ProfileForm.qml
    ProfileDetailsItem.qml
    LessonSelector.qml
    LessonPreview.qml
    LessonLockedNotice.qml
    InitialProfileForm.qml

    TrainingScreen.qml
    TrainingScreenToolbar.qml
    TrainingScreenMenuOverlay.qml
    TrainingWidget.qml
    TrainingLine.qml
    EllapsedTimeMeter.qml
    CharactersPerMinuteMeter.qml
    AccuracyMeter.qml
    AnalogEllapsedTimeMeter.qml
    AnalogCharactersPerMinuteMeter.qml
    AnalogAccuracyMeter.qml
    DigitalMeter.qml
    Keyboard.qml
    KeyItem.qml
    KeyLabel.qml

    ScoreScreen.qml

    InformationTable.qml
    MessageBox.qml
    ListItem.qml
    SheetDialog.qml

    KeyboardLayoutEditor.qml
    SelectionRectangle.qml
    SelectionGrip.qml
)

install(FILES ${ktouch_qml_files} DESTINATION ${DATA_INSTALL_DIR}/ktouch/qml)
