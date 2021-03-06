// Copyright Steinwurf ApS 2015.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#if !defined(KODO_PYTHON_DISABLE_RLNC)

// boost/python.hpp should be the first include in every .cpp file to work
// around this bug: https://bugs.python.org/issue10910
#include <boost/python.hpp>

#include <kodo_rlnc/coders.hpp>

#include "create_helpers.hpp"
#include "symbol_decoding_status_updater_methods.hpp"
#include "systematic_encoder_methods.hpp"

namespace kodo_python
{
template<>
struct extra_encoder_methods<kodo_rlnc::encoder>
{
    template<class EncoderClass>
    extra_encoder_methods(EncoderClass& encoder_class)
    {
        using boost::python::arg;

        encoder_class
        .def("set_seed",
             &EncoderClass::wrapped_type::set_seed, arg("seed"),
             "Set the seed of the coefficient generator.\n\n"
             "\t:param seed: The seed value.\n")
        .def("set_density",
             &EncoderClass::wrapped_type::set_density, arg("density"),
             "Set the density of the coefficients generated.\n\n"
             "\t:param density: The coefficients density.\n")
        .def("density",
             &EncoderClass::wrapped_type::density,
             "Get the density of the coefficients generated.\n\n"
             "\t:returns: The density of the generator.\n");

        systematic_encoder_methods(encoder_class);
    }
};

template<>
struct extra_decoder_methods<kodo_rlnc::decoder>
{
    template<class DecoderClass>
    extra_decoder_methods(DecoderClass& decoder_class)
    {
        using boost::python::arg;

        decoder_class
        .def("set_seed",
             &DecoderClass::wrapped_type::set_seed, arg("seed"),
             "Set the seed of the coefficient generator.\n\n"
             "\t:param seed: The seed value.\n")
        .def("write_payload",
             &decoder_write_payload<typename DecoderClass::wrapped_type>,
             "Generate a recoded symbol.\n\n"
             "\t:returns: The recoded symbol.\n")
        .def("is_partially_complete",
             &DecoderClass::wrapped_type::is_partially_complete,
             "Check if some symbols in the decoder are fully decoded even\n"
             "though the full data block has not been sent.\n\n"
             "\t:returns: True if the decoding matrix is partially complete.\n");

        symbol_decoding_status_updater_methods(decoder_class);
    }
};

template<>
struct extra_factory_methods<kodo_rlnc::encoder>
{
    template<class FactoryClass>
    extra_factory_methods(FactoryClass& factory_class)
    {
        using boost::python::arg;

        factory_class
        .def("set_coding_vector_format",
             &FactoryClass::wrapped_type::set_coding_vector_format,
             arg("format"),
             "Set the coding vector format for encoded payloads.\n\n"
             "\t:param format: The selected coding vector format,\n");
    }
};

template<>
struct extra_factory_methods<kodo_rlnc::pure_recoder>
{
    template<class FactoryClass>
    extra_factory_methods(FactoryClass& factory_class)
    {
        using boost::python::arg;

        factory_class
        .def("recoder_symbols",
             &FactoryClass::wrapped_type::recoder_symbols,
             "Return the the number of internal symbols that should be\n"
             "stored in the recoder.\n\n"
             "\t:returns: The number of recoder symbols.\n")
        .def("set_recoder_symbols",
             &FactoryClass::wrapped_type::set_recoder_symbols,
             arg("symbols"),
             "Set the number of recoder symbols for coders to be created\n"
             "with this factory.\n\n"
             "\t:param symbols: The number of recoder symbols to use.\n");
    }
};

void create_rlnc_stacks()
{
    using namespace boost::python;

    enum_<kodo_rlnc::coding_vector_format>("coding_vector_format")
    .value("full_vector", kodo_rlnc::coding_vector_format::full_vector)
    .value("seed", kodo_rlnc::coding_vector_format::seed)
    .value("sparse_seed", kodo_rlnc::coding_vector_format::sparse_seed);

    create_factory_and_encoder<kodo_rlnc::encoder>("RLNCEncoder");
    create_factory_and_decoder<kodo_rlnc::decoder>("RLNCDecoder");
    create_factory_and_recoder<kodo_rlnc::pure_recoder>("RLNCPureRecoder");
}
}

#endif
