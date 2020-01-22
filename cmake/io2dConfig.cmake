#find_path(LIBIO2D_INCLUDE_DIR
#	NAMES io2d libio2d libio2d_core
#	PATH "/home/ddonohue/cpp/nanodegree/P0267_RefImpl"
#	)

find_library(io2d
	PATHS "/home/ddonohue/cpp/nanodegree/P0267_RefImpl"
	)

#add_subdirectory(/home/ddonohue/cpp/nanodegree/P0267_RefImpl/Build/P0267_RefImpl/P0267_RefImpl)
#list(APPEND EXTRA_LIBS P0267_RefImpl)

#target_link_libraries(OSM_A_star_search PUBLIC P0267_RefImpl)
