// Copyright Paul A. Bristow 2016, 2017.
// Copyright John Maddock 2016.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// test_lambertw.cpp
//! \brief Basic sanity tests for Lambert W function plog or productlog using algorithm from Thomas Luu.

#include <boost/math/concepts/real_concept.hpp> // for real_concept
#define BOOST_TEST_MAIN
#define BOOST_LIB_DIAGNOSTIC

#include <boost/test/unit_test.hpp> // Boost.Test
#include <boost/test/floating_point_comparison.hpp>
#include "test_value.hpp"  // for create_test_value

#include <boost/multiprecision/cpp_dec_float.hpp> // boost::multiprecision::cpp_dec_float_50
using boost::multiprecision::cpp_dec_float_50;
#ifdef BOOST_HAS_FLOAT128
#include <boost/multiprecision/float128.hpp>
#endif

#include <boost/fixed_point/fixed_point.hpp>

#include <boost/math/special_functions/lambert_w.hpp> // for productlog function.
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/type_traits/is_constructible.hpp>

#include <limits>
#include <cmath>
#include <typeinfo>
#include <iostream>
#include <type_traits>

static const unsigned int noof_tests = 2;

static const boost::array<const char*, noof_tests> test_data =
{{
  "1",
  "6"
}}; // array test_data

//static const boost::array<const char*, noof_tests> test_expected =
//{ {
//    BOOST_MATH_TEST_VALUE(T, 0.56714329040978387299996866221035554975381578718651), // Output from https://www.wolframalpha.com/input/?i=productlog(1)
//    BOOST_MATH_TEST_VALUE(T, 1.432404775898300311234078007212058694786434608804302025655) // Output from https://www.wolframalpha.com/input/?i=productlog(6)

//  } }; // array test_data

//! Show information about build, architecture, address model, platform, ...
std::string show_versions()
{
  std::ostringstream message;

  message << "Program: " << __FILE__ << "\n";
#ifdef __TIMESTAMP__
  message << __TIMESTAMP__;
#endif
  message << "\nBuildInfo:\n" "  Platform " << BOOST_PLATFORM;
  // http://stackoverflow.com/questions/1505582/determining-32-vs-64-bit-in-c
#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__) ) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
#define IS64BIT 1
  message << ", 64-bit.";
#else
#define IS32BIT 1
  message << ", 32-bit.";
#endif

  message << "\n  Compiler " BOOST_COMPILER;
#ifdef BOOST_MSC_VER
#ifdef _MSC_FULL_VER
  message << "\n  MSVC version " << BOOST_STRINGIZE(_MSC_FULL_VER) << ".";
#endif
#ifdef __WIN64
  mess age << "\n WIN64" << std::endl;
#endif // __WIN64
#ifdef _WIN32
  message << "\n WIN32" << std::endl;
#endif  // __WIN32
#endif
#ifdef __GNUC__
  //PRINT_MACRO(__GNUC__);
  //PRINT_MACRO(__GNUC_MINOR__);
  //PRINT_MACRO(__GNUC_PATCH__);
  std::cout << "GCC " << __VERSION__ << std::endl;
  //PRINT_MACRO(LONG_MAX);
#endif // __GNUC__

  message << "\n  STL " << BOOST_STDLIB;

  message << "\n  Boost version " << BOOST_VERSION / 100000 << "." << BOOST_VERSION / 100 % 1000 << "." << BOOST_VERSION % 100;

#ifdef BOOST_HAS_FLOAT128
  message << ",  BOOST_HAS_FLOAT128" << std::endl;
#endif
  message << std::endl;
  return message.str();
} // std::string versions()


// ProductLog[-0.367879441171442321595523770161460867445811131031767834]

template <class RealType>
void test_spots(RealType)
{
  // (Unused Parameter value, arbitrarily zero, only communicates the floating point type).
  // test_spots(0.F); test_spots(0.); test_spots(0.L);

//   // Check some bad parameters to the function,
//#ifndef BOOST_NO_EXCEPTIONS
//  BOOST_MATH_CHECK_THROW(boost::math::normal_distribution<RealType> nbad1(0, 0), std::domain_error); // zero sd
//  BOOST_MATH_CHECK_THROW(boost::math::normal_distribution<RealType> nbad1(0, -1), std::domain_error); // negative sd
//#else
//  BOOST_MATH_CHECK_THROW(boost::math::normal_distribution<RealType>(0, 0), std::domain_error); // zero sd
//  BOOST_MATH_CHECK_THROW(boost::math::normal_distribution<RealType>(0, -1), std::domain_error); // negative sd
//#endif

  using boost::math::productlog;

  using boost::math::constants::exp_minus_one;
  RealType tolerance = boost::math::tools::epsilon<RealType>() * 5; // 5 eps as a fraction.
  std::cout << "Testing type " << typeid(RealType).name() << std::endl;
  std::cout << "Tolerance " << tolerance << std::endl;
  std::cout << "Precision " << std::numeric_limits<RealType>::digits10 << " decimal digits." << std::endl;
  std::cout.precision(std::numeric_limits<RealType>::digits10);
  std::cout << std::showpoint << std::endl; // show trailing significant zeros.
  std::cout << "-exp(-1) = " << -exp_minus_one<RealType>() << std::endl;

  std::cout.precision(std::numeric_limits <RealType>::max_digits10);

  RealType test_value = BOOST_MATH_TEST_VALUE(RealType, -0.36787944117144232159552377016146086744581113103176);
  RealType expected_value = BOOST_MATH_TEST_VALUE(RealType, -1.);

  test_value = -boost::math::constants::exp_minus_one<RealType>(); // -exp(-1) = -0.367879450
  std::cout << "test " << test_value << ", expected productlog = " << expected_value << std::endl;

  BOOST_CHECK_CLOSE_FRACTION( // Check -exp(-1) = -0.367879450 = -1
    productlog(test_value),
    expected_value,
    tolerance);  // OK

  BOOST_CHECK_CLOSE_FRACTION(  // Check -exp(-1) = -0.367879450 = -1
    productlog(BOOST_MATH_TEST_VALUE(RealType, -0.36787944117144232159552377016146086744581113103176)),
    BOOST_MATH_TEST_VALUE(RealType, -1.),
    tolerance);

  BOOST_CHECK_CLOSE_FRACTION(
    productlog(BOOST_MATH_TEST_VALUE(RealType, 1.)),
    BOOST_MATH_TEST_VALUE(RealType, 0.56714329040978387299996866221035554975381578718651),
   // Output from https://www.wolframalpha.com/input/?i=productlog(1)
   tolerance);

   //Tests with some spot values computed using
   //https://www.wolframalpha.com
   //For example: N[ProductLog[-1], 50] outputs:
   //1.3267246652422002236350992977580796601287935546380

  BOOST_CHECK_CLOSE_FRACTION(productlog(BOOST_MATH_TEST_VALUE(RealType, 2.)),
    BOOST_MATH_TEST_VALUE(RealType, 0.852605502013725491346472414695317466898453300151403508772),
    // Output from https://www.wolframalpha.com/input/?i=productlog(2.)
    tolerance);

  BOOST_CHECK_CLOSE_FRACTION(productlog(BOOST_MATH_TEST_VALUE(RealType, 3.)),
    BOOST_MATH_TEST_VALUE(RealType, 1.049908894964039959988697070552897904589466943706341452932),
    // Output from https://www.wolframalpha.com/input/?i=productlog(3.)
    tolerance);

  BOOST_CHECK_CLOSE_FRACTION(productlog(BOOST_MATH_TEST_VALUE(RealType, 5.)),
    BOOST_MATH_TEST_VALUE(RealType, 1.326724665242200223635099297758079660128793554638047479789),
    // Output from https://www.wolframalpha.com/input/?i=productlog(0.5)
    tolerance);


  BOOST_CHECK_CLOSE_FRACTION(productlog(BOOST_MATH_TEST_VALUE(RealType, 0.5)),
    BOOST_MATH_TEST_VALUE(RealType, 0.351733711249195826024909300929951065171464215517111804046),
    // Output from https://www.wolframalpha.com/input/?i=productlog(0.5)
    tolerance);


  BOOST_CHECK_CLOSE_FRACTION(productlog(BOOST_MATH_TEST_VALUE(RealType, 6.)),
    BOOST_MATH_TEST_VALUE(RealType, 1.432404775898300311234078007212058694786434608804302025655),
    // Output from https://www.wolframalpha.com/input/?i=productlog(6)
    tolerance);

  BOOST_CHECK_CLOSE_FRACTION(productlog(BOOST_MATH_TEST_VALUE(RealType, 100.)),
    BOOST_MATH_TEST_VALUE(RealType, 3.3856301402900501848882443645297268674916941701578),
    // Output from https://www.wolframalpha.com/input/?i=productlog(100)
    tolerance);

  // TODO Fails here with really big x.
  /*  BOOST_CHECK_CLOSE_FRACTION(productlog(BOOST_MATH_TEST_VALUE(RealType, 1.0e6)),
    BOOST_MATH_TEST_VALUE(RealType, 11.383358086140052622000156781585004289033774706019),
    // Output from https://www.wolframalpha.com/input/?i=productlog(1e6)
    // tolerance * 1000); // fails exceeds 0.00015258789063
    tolerance * 1000);

  1>i:/modular-boost/libs/math/test/test_lambert_w.cpp(195):
  error : in "test_main":
  difference{2877.54} between
  productlog(create_test_value<RealType>( 1.0e6L, "1.0e6", boost::mpl::bool_< std::numeric_limits<RealType>::is_specialized && (std::numeric_limits<RealType>::radix == 2) && (std::numeric_limits<RealType>::digits <= std::numeric_limits<long double>::digits) && boost::is_convertible<long double, RealType>::value>(), boost::mpl::bool_< boost::is_constructible<RealType, const char*>::value>()))
{
  32767.399857
}
and

create_test_value<RealType>( 11.383358086140052622000156781585004289033774706019L, "11.383358086140052622000156781585004289033774706019", boost::mpl::bool_< std::numeric_limits<RealType>::is_specialized && (std::numeric_limits<RealType>::radix == 2) && (std::numeric_limits<RealType>::digits <= std::numeric_limits<long double>::digits) && boost::is_convertible<long double, RealType>::value>(), boost::mpl::bool_< boost::is_constructible<RealType, const char*>::value>())
{
  11.383346558
}
exceeds 0.00015258789063

  */


  BOOST_CHECK_CLOSE_FRACTION(productlog(BOOST_MATH_TEST_VALUE(RealType, 0.0)),
    BOOST_MATH_TEST_VALUE(RealType, 0.),
    tolerance);

  // This is very close to the limit of -exp(1) * x
  // (where the result has a non-zero imaginary part).
  test_value = -exp_minus_one<RealType>();
  test_value += (std::numeric_limits<RealType>::epsilon() * 100);
  expected_value = BOOST_MATH_TEST_VALUE(RealType, -1.0);

  // std::cout << test_value << std::endl; // -0.367879

  // Fatal error here with float.
  //BOOST_CHECK_CLOSE_FRACTION(productlog(test_value),
  //  expected_value,
  //  tolerance);

  /*
  i:/modular-boost/libs/math/test/test_lambert_w.cpp(224):
  error : in "test_main":

  difference{1e+67108864} between productlog(test_value)
  {
    0
  } and create_test_value<RealType>(
   -0.99845210378072725931829498030640227316856835774851L,
  "-0.99845210378072725931829498030640227316856835774851",
  boost::mpl::bool_< std::numeric_limits<RealType>::is_specialized && (std::numeric_limits<RealType>::radix == 2) && (std::numeric_limits<RealType>::digits <= std::numeric_limits<long double>::digits) && boost::is_convertible<long double, RealType>::value>(), boost::mpl::bool_< boost::is_constructible<RealType, const char*>::value>())
  {
    -0.99845210378072725931829498030640227316856835774851
  }

  exceeds 5e-47
  */


   /* Goes really haywire here close to singularity.

  test_value = BOOST_MATH_TEST_VALUE(RealType, -0.367879);
  BOOST_CHECK_CLOSE_FRACTION(productlog(test_value),
    BOOST_MATH_TEST_VALUE(RealType, -0.99845210378072725931829498030640227316856835774851),
    // Output from https://www.wolframalpha.com/input/?i=productlog(2.)
    // N[productlog(-0.367879), 50] = -0.99845210378072725931829498030640227316856835774851
    tolerance * 100);

  I:/modular-boost/libs/math/test/test_lambert_w.cpp(267): error : in "test_main":
  difference
  {
    2.87298e+26
  }
  between productlog(test_value)
  {
    -2.86853068e+26
  }
  and create_test_value<RealType>( -0.99845210378072725931829498030640227316856835774851L, "-0.99845210378072725931829498030640227316856835774851", boost::mpl::bool_< std::numeric_limits<RealType>::is_specialized && (std::numeric_limits<RealType>::radix == 2) && (std::numeric_limits<RealType>::digits <= std::numeric_limits<long double>::digits) && boost::is_convertible<long double, RealType>::value>(), boost::mpl::bool_< boost::is_constructible<RealType, const char*>::value>())
  {
    -0.998452127
  }
  exceeds 5.96046448e-05

  */


  // Checks on input that should throw.

 /* This should throw when implemented.
  BOOST_CHECK_CLOSE_FRACTION(productlog(-0.5),
  BOOST_MATH_TEST_VALUE(RealType, ),
    // Output from https://www.wolframalpha.com/input/?i=productlog(-0.5)
    tolerance);
   */
} //template <class RealType>void test_spots(RealType)

BOOST_AUTO_TEST_CASE(test_main)
{
  BOOST_MATH_CONTROL_FP;
  // BOOST_TEST_MESSAGE( output only appears if command line has --log_level="message"
  // or call set_threshold_level function:
  boost::unit_test_framework::unit_test_log.set_threshold_level(boost::unit_test_framework::log_messages);
  BOOST_TEST_MESSAGE("Test Lambert W function.");
  BOOST_TEST_MESSAGE(show_versions());  // Full version of Boost, STL and compiler info.

  // Fundamental built-in types:
  test_spots(0.0F); // float
  test_spots(0.0); // double
  if (sizeof(long double) > sizeof (double))
  { // Avoid pointless re-testing if double and long double are identical (for example, MSVC).
    test_spots(0.0L); // long double
  }
  // Built-in/fundamental Quad 128-bit type, if available.
  #ifdef BOOST_HAS_FLOAT128
  test_spots(static_cast<boost::multiprecision::float128>(0));
#endif

  // Multiprecision types:
  test_spots(static_cast<boost::multiprecision::cpp_dec_float_50>(0));
  test_spots(static_cast<boost::multiprecision::cpp_bin_float_double_extended>(0));  // Fails GCC gnu++11 and gnu ++14but OK -std=c++11 and 14.
  test_spots(static_cast<boost::multiprecision::cpp_bin_float_quad>(0)); // Fails GCC gnu++11 and ++14 but OK -std=c++11 and 14.

  // Fixed-point types:
  test_spots(static_cast<boost::fixed_point::negatable<15,-16> >(0));

  //test_spots(boost::math::concepts::real_concept(0.1));  // "real_concept" - was OK.
  // link fails - need to add as a permanent constant.
  /*
  test_lambert_w.obj : error LNK2001 : unresolved external symbol "private: static class boost::math::concepts::real_concept __cdecl boost::math::constants::detail::constant_expminusone<class boost::math::concepts::real_concept>::compute<0>(void)" (? ? $compute@$0A@@?$constant_expminusone@Vreal_concept@concepts@math@boost@@@detail@constants@math@boost@@CA?AVreal_concept@concepts@34@XZ)
  */



} // BOOST_AUTO_TEST_CASE( test_main )

  /*

  Output:

  tolerance just one epsilon fails for  Lambert W (-0.367879) = -0.998452

  1>------ Rebuild All started: Project: test_lambertW, Configuration: Release x64 ------
1>  test_lambertW.cpp
1>  Linking to lib file: libboost_unit_test_framework-vc140-mt-1_63.lib
1>  Generating code
1>  All 1827 functions were compiled because no usable IPDB/IOBJ from previous compilation was found.
1>  Finished generating code
1>  test_lambertW.vcxproj -> J:\Cpp\Misc\x64\Release\test_lambertW.exe
1>  test_lambertW.vcxproj -> J:\Cpp\Misc\x64\Release\test_lambertW.pdb (Full PDB)
1>  Running 1 test case...
1>  Platform: Win32
1>  Compiler: Microsoft Visual C++ version 14.0
1>  STL     : Dinkumware standard library version 650
1>  Boost   : 1.63.0
1>  Tests run with Microsoft Visual C++ version 14.0, Dinkumware standard library version 650, Boost PlatformWin32, MSVC compiler 190024123
1>  Tolerance 5.96046e-07
1>  Precision 6 decimal digits.
1>  Iteration #0, w0 0.577547, w1 = 0.567144, difference = 0.0289948, relative 0.018344
1>  Iteration #1, w0 0.567144, w1 = 0.567143, difference = 9.53674e-07, relative 5.96046e-07
1>  Final 0.567143 after 2 iterations, difference = 0
1>  Tolerance 1.11022e-15
1>  Precision 15 decimal digits.
1>  Iteration #0, w0 0.577547206058041, w1 = 0.567143616915443, difference = 0.0289944962755619, relative 0.018343835374856
1>  Iteration #1, w0 0.567143616915443, w1 = 0.567143290409784, difference = 9.02208135089566e-07, relative 5.75702234328901e-07
1>  Final 0.567143290409784 after 2 iterations, difference = 0
1>  Tolerance 5e-49
1>  Precision 50 decimal digits.
1>  Iteration #0, w0 0.57754720605804066335708515521786300470367806666917, w1 = 0.5671436169154433808085244686233766561058069997742, difference = 0.028994496275562314267349048621807448516020440172019, relative 0.018343835374855986875831315372982269135605651729877
1>  Iteration #1, w0 0.5671436169154433808085244686233766561058069997742, w1 = 0.56714329040978387301011426674463910433535588015473, difference = 9.0220813517014912275046839365249789421357156514914e-07, relative 5.7570223438927562537725655919526667550991229663452e-07
1>  Iteration #2, w0 0.56714329040978387301011426674463910433535588015473, w1 = 0.56714329040978387299996866221035554975381578718651, difference = 2.8034566117436458709490133985425058248551576808341e-20, relative 1.7888961583152904127717080041769389899099419098831e-20
1>  Final 0.56714329040978387299996866221035554975381578718651 after 3 iterations, difference = 0
1>
1>  *** No errors detected
========== Rebuild All: 1 succeeded, 0 failed, 0 skipped ==========


  */




