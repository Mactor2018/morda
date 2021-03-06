include prorab.mk


this_name := morda

this_soname := $(shell cat $(d)/soname.txt)

this_srcs += $(call prorab-src-dir,morda)

this_cxxflags += -Wall #enable all warnings
this_cxxflags += -Wno-comment #no warnings on nested comments
this_cxxflags += -funsigned-char #the 'char' type is unsigned
this_cxxflags += -Wnon-virtual-dtor #warn if base class has non-virtual destructor
this_cxxflags += -fno-operator-names #do not treat 'and', 'bitand','bitor','compl','not','or','xor' as keywords
#TODO: uncomment this when problem with "function mangling name will be changed" warning issued by g++ 7
#this_cxxflags += -Werror #treat warnings as errors
this_cxxflags += -Wfatal-errors # stop on first error encountered
this_cxxflags += -fstrict-aliasing #strict aliasing (in order to comply with the c++ standard more strictly)
this_cxxflags += -g #include debugging symbols
this_cxxflags += -std=c++11
#this_cxxflags += -Wno-noexcept-type


this_objcflags := 

ifeq ($(debug),true)
    this_cxxflags += -DDEBUG
#    this_cxxflags += -DM_MORDA_RENDER_WIDGET_BORDERS
    this_objcflags += -DDEBUG
else
    this_cxxflags += -O3
endif

ifeq ($(os), linux)
    this_cxxflags += -fPIC #generate position independent code
    this_cxxflags += `pkg-config --cflags freetype2`
    this_ldlibs += -lpthread `pkg-config --libs freetype2` -lz
else ifeq ($(os), windows)
#    this_cxxflags += -Wno-format #due to bugs in some g++ versions it warns about format when not needed, so disable format warnings
#    this_cxxflags += -I/usr/include -I/usr/local/include/ -I/usr/local/include/freetype2
#    this_ldflags += -L/usr/local/lib -L/usr/lib

    #NOTE: order matters in MinGW, libs referenced by other libs should go after those libs which reference them.
#    this_ldlibs += -lfreetype -lz -lopengl32 -lglew32 -lgdi32
else ifeq ($(os), macosx)
    this_cxxflags += -stdlib=libc++ #this is needed to be able to use c++11 std lib
    this_cxxflags += -I/usr/local/include/freetype2
    this_ldlibs += -lc++
    this_ldlibs += -lfreetype
endif

this_ldlibs += -lpapki -lstob -lsvgdom -lsvgren -lunikod -lpng -ljpeg -lstdc++ -lm

$(eval $(prorab-build-lib))
