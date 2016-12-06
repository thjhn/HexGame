cc_library(
    name = "hexfield",
    srcs = ["hexfield.cpp"],
    hdrs = ["hexfield.h"],
)

cc_library(
    name = "hexmove",
    srcs = ["hexmove.cpp"],
    hdrs = ["hexmove.h"],
)

cc_library(
    name = "hexplayer",
    srcs = ["hexplayer.cpp"],
    hdrs = ["hexplayer.h"],
    deps = [
	":hexmove",
    ]
)

cc_library(
    name = "hexengine",
    srcs = ["hexengine.cpp"],
    hdrs = ["hexengine.h"],
    deps = [
	":hexfield",
	":hexplayer",
    ]
)

cc_library(
    name = "hexlearner",
    srcs = ["hexlearner.cpp"],
    hdrs = ["hexlearner.h"],
    deps = [
	":hexmove",
	":hexplayer",
	":hexengine",
	"//tensorflow/core:tensorflow",
    ]
)

cc_library(
    name = "hexgui",
    srcs = ["hexgui.cpp"],
    hdrs = ["hexgui.h"],
    deps = [
	":hexmove",
	":hexplayer",
	":hexengine",
    ]
)

cc_library(
    name = "hexrandomplayer",
    srcs = ["hexrandomplayer.cpp"],
    hdrs = ["hexrandomplayer.h"],
    deps = [
	":hexmove",
	":hexplayer",
	":hexengine",
    ]
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [
	":hexengine",
	":hexgui",
	":hexlearner",
    ]
)
