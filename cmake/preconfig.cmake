
if(NOT TARGET glm)
	find_dependency(glm CONFIG)
endif()

if(NOT TARGET ez::meta)
	find_dependency(ez-meta CONFIG)
endif()

if(NOT TARGET xxHash::xxhash)
	find_dependency(xxHash CONFIG)
endif()