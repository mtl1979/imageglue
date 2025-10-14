qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES en fi)

if(MSVC)
  set(RCFILE imageglue.rc)

  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrt")
endif()

qt_add_executable(ImageGlue
  imageview.cpp
  main.cpp
  mainwindow.cpp
  preview.cpp
  util.cpp
  imageglue.qrc
  ${RCFILE}
)

target_include_directories(ImageGlue PRIVATE Qt6::Core Qt6::Widgets)
target_link_libraries(ImageGlue PRIVATE Qt6::Core Qt6::Widgets ZLIB::ZLIB)
set_target_properties(ImageGlue PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

install(TARGETS ImageGlue)
qt_generate_deploy_app_script(
    TARGET ImageGlue
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

qt_add_translations(ImageGlue QM_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} QM_FILES_OUTPUT_VARIABLE qm_files)
install(FILES ${qm_files} DESTINATION "translations")