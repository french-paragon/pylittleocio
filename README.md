# pylittleocio
This is a small opencolorio binding for python, implementing the minimal set of functionnality to perform color transforms

I created this module because the main ocio biding for python is not always the most convinient to use, especially when one just want to apply a color transform before displaying an image. The module contain a single class `ColorTransformer`, which takes four argument in its constructor, the ocio config file, the input color space, the view color space and the view transform. For example a color transform can be created with:

	color_transform = pylittleocio.ColorTransformer('config.ocio', 'Linear', 'sRGB', 'Filmic')

The transform can then be applied in place on a float32 numpy array:

	color_transform.applyTransform(colorImg)

## Build

To build the module you need CMake, make, pybind11, python (with the devellopement files) and opencolorio (with the devellopement files). Just use CMake as usual, the configuration should be automatic. I recommend you use a structure with a top level build and src directories and the corresponding subdirectories :

	cd build; mkdir build pylittleociorelsease; cd pylittleociorelsease
	cmake ../../src/pylittleocio
	make
	make install
