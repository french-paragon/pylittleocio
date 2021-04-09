#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <OpenColorIO/OpenColorIO.h>
namespace OCIO = OCIO_NAMESPACE;

namespace py = pybind11;

#define xstr(a) str(a)
#define str(a) #a


struct OcioColorTransformerData;

class OcioColorTransformer
{
public:
        OcioColorTransformer(std::string const& configFile, std::string const& inColorSpace, std::string const& outView, std::string const& displayView);

        ~OcioColorTransformer();

        void applyTransform(pybind11::array_t<float> input);

private:
        OcioColorTransformerData* _dats;
};



struct OcioColorTransformerData {
	OCIO::ConstConfigRcPtr _config;
	OCIO::ConstProcessorRcPtr _processor;
};

OcioColorTransformer::OcioColorTransformer(std::string const& configFile, std::string const& inColorSpace, std::string const& outView, std::string const& displayView) {

	OCIO::ConstConfigRcPtr config = OCIO::Config::CreateFromFile(configFile.c_str());

	OCIO::ConstProcessorRcPtr processor = config->getProcessor(inColorSpace.c_str(), config->getDisplayColorSpaceName(displayView.c_str(), outView.c_str()));

	_dats = new OcioColorTransformerData({
											 config,
											 processor
										 });
}


OcioColorTransformer::~OcioColorTransformer() {
	delete _dats;
}

void OcioColorTransformer::applyTransform(pybind11::array_t<float> input) {

	std::vector<long> im_shape = input.request().shape;
	std::vector<long> im_strides = input.request().strides;

	if (im_shape.size() != 3) {
		throw py::index_error("Expected an image with 3 dimensions, got " + std::to_string(im_shape.size()) + " !");
	}

	if (im_shape[2] != 3 and im_shape[2] != 4) {
		throw py::index_error("Expected an image with 3 or 4 channels, got " + std::to_string(im_shape[2]) + " !");
	}

	OCIO::PackedImageDesc desc(input.mutable_data(), im_shape[1], im_shape[0], im_shape[2], im_strides[2], im_strides[1], im_strides[0]);

	if (im_shape[2] == 3) {
		_dats->_processor->apply(desc);
	}
}


PYBIND11_MODULE(LIB_NAME, m) {
	m.doc() = "pybind11 " xstr(LIB_NAME) " module.";
	
	py::class_<OcioColorTransformer>(m, "ColorTransformer")
			.def(py::init<const std::string &, const std::string &, const std::string &, const std::string &>())
			.def("applyTransform", &OcioColorTransformer::applyTransform);
	
}
