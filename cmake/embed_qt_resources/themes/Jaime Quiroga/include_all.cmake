set(JAIME_QUIROGA_THEME_IMAGES)

macro(make_jaime_quiroga_theme NAME)
qt_add_resources(DllExportsViewer "${NAME}_jaime_quiroga_theme"
        PREFIX "RC_THEMES"
        BASE "resources/themes/"
        FILES
        ${JAIME_QUIROGA_THEME_IMAGES}
        resources/themes/Jaime\ Quiroga/${NAME}.qss
)
endmacro()

make_jaime_quiroga_theme(Aqua)
