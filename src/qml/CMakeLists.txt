set(ktouch_qml_files
    main.qml

    HomeScreen.qml
    CourseSelector.qml
    CoursePage.qml
    CourseDescriptionItem.qml
    NoCoursesMessage.qml
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
    ElapsedTimeMeter.qml
    CharactersPerMinuteMeter.qml
    AccuracyMeter.qml
    AnalogElapsedTimeMeter.qml
    AnalogCharactersPerMinuteMeter.qml
    AnalogAccuracyMeter.qml
    DigitalMeter.qml
    Keyboard.qml
    KeyItem.qml
    KeyLabel.qml
    KeyboardUnavailableNotice.qml

    ScoreScreen.qml
    LearningProgressGraph.qml

    InformationTable.qml
    InfoItem.qml
    MessageBox.qml
    ListItem.qml
    SheetDialog.qml
    Baloon.qml

    KeyboardLayoutEditor.qml
    SelectionRectangle.qml
    SelectionGrip.qml
)

install(FILES ${ktouch_qml_files} DESTINATION ${DATA_INSTALL_DIR}/ktouch/qml)
