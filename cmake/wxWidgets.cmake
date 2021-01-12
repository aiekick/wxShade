set(wxBUILD_SHARED OFF)
set(wxBUILD_INSTALL OFF)
set(wxBUILD_DEMOS OFF)
set(wxBUILD_USE_STATIC_RUNTIME ON)
set(wxUSE_GUI ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/wxWidgets)

set_target_properties(clean_cotire PROPERTIES FOLDER 3rdparty/wxWidgets/cotire)
set(WX_LIBS 
	wxadv wxaui wxbase wxcore 
	wxgl wxhtml wxmedia wxnet 
	wxpropgrid wxqa wxribbon 
	wxrichtext wxstc wxwebview 
	wxxml wxxrc)
set_target_properties(${WX_LIBS} PROPERTIES FOLDER 3rdparty/wxWidgets/Libs)
set(WX_EXTERNAL 
	wxexpat wxjpeg wxpng 
	wxregex wxscintilla 
	wxtiff wxzlib)
set_target_properties(${WX_EXTERNAL} PROPERTIES FOLDER 3rdparty/wxWidgets/External)
set_target_properties(wxrc PROPERTIES FOLDER 3rdparty/wxWidgets/Utils)

set(WX_LIBRARIES 
	${WX_LIBS}
	${WX_EXTERNAL})