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
    LessonSelectorBase.qml
    LessonSelector.qml
    CustomLessonSelector.qml
    LessonPreview.qml
    LessonLockedNotice.qml
    InitialProfileForm.qml

    TrainingScreen.qml
    TrainingScreenToolbar.qml
    TrainingScreenMenuOverlay.qml
    TrainingWidget.qml
    StatBox.qml
    ElapsedTimeMeter.qml
    CharactersPerMinuteMeter.qml
    AccuracyMeter.qml
    Meter.qml
    Keyboard.qml
    KeyItem.qml
    KeyLabel.qml
    KeyboardUnavailableNotice.qml

    ScoreScreen.qml
    LearningProgressChart.qml

    DetailedRadioButton.qml
    InformationTable.qml
    InfoItem.qml
    InlineToolbar.qml
    MessageBox.qml
    ListItem.qml
    SheetDialog.qml
    Balloon.qml

    KeyboardLayoutEditor.qml
    SelectionRectangle.qml
    SelectionGrip.qml
)

install(FILES ${ktouch_qml_files} DESTINATION ${DATA_INSTALL_DIR}/ktouch/qml)
