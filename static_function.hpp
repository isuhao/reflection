#ifndef STATIC_FUNCTION_HPP
#define STATIC_FUNCTION_HPP
#include "../misc/misc.hpp"
#include "reflection.hpp"
#define DECLARE_POSSIBLE_STATIC_FUNCTION( NAME ) \
	template< typename T, typename SELF, typename ... R > \
	constexpr static bool has_static_function( \
		typename std::enable_if \
		< \
			std::is_same \
			< \
				T, \
				BOOST_PP_CAT( NAME, _tag ) \
			>::value, \
			typename std::add_pointer \
			< \
				decltype( SELF::NAME( misc::construct< R >( ) ... ) ) \
			>::type \
		>::type ) \
	{ return true; } \
	template< typename T, typename SELF, typename ... R > \
	typename std::enable_if \
	< \
		std::is_same \
		< \
			T, \
			BOOST_PP_CAT( NAME, _tag ) \
		>::value, \
		decltype( SELF::NAME( misc::construct< R >( ) ... ) ) \
	>::type \
	static call_static_function( const R & ...  r ) { return SELF::NAME( r ... ); }
#define HAS_STATIC_FUNCTION( TYPE, NAME, ARGUMENT_TUPLE ) \
	has_static_function \
	< \
		TYPE, \
		BOOST_PP_CAT( NAME, _tag ) \
		EXPAND_TUPLE_ARGUMENT( ARGUMENT_TUPLE ) \
	>::value
#define STATIC_FUNCTION_RETURN_TYPE( TYPE, NAME, ARGUMENT_TUPLE ) \
	static_function_return_type \
	< \
		TYPE, \
		BOOST_PP_CAT( NAME, _tag ) \
		EXPAND_TUPLE_ARGUMENT( ARGUMENT_TUPLE ) \
	>::type
#define CALL_STATIC_FUNCTION( TYPE, NAME, ARGUMENT_TUPLE ) \
	call_static_function \
	< \
		TYPE, \
		BOOST_PP_CAT( NAME, _tag ) \
		EXPAND_TUPLE_ARGUMENT( \
			BOOST_PP_SEQ_TO_TUPLE( \
				BOOST_PP_SEQ_TRANSFORM( \
					DECLTYPE_HELPER, \
					_, \
					BOOST_PP_TUPLE_TO_SEQ( ARGUMENT_TUPLE ) ) ) ) \
	>( )( BOOST_PP_TUPLE_ENUM( ARGUMENT_TUPLE ) )
template< typename TYPE, typename NAME, typename ... ARG >
struct static_function_return_type
{
	typedef decltype(
			TYPE::template call_static_function< NAME, TYPE >(
				misc::construct< ARG >( ) ... ) ) type;
};
template< typename TYPE, typename NAME >
struct static_function_return_type< TYPE, NAME, void >
{
	typedef decltype(
			TYPE::template call_static_function< NAME, TYPE >( ) ) type;
};
template< typename TYPE, typename NAME, typename ... ARG >
struct has_static_function { static constexpr bool value = TYPE::template has_static_function< NAME, TYPE, ARG ... >( nullptr ); };
template< typename TYPE, typename NAME >
struct has_static_function< TYPE, NAME, void > { static constexpr bool value = TYPE::template has_static_function< NAME, TYPE >( nullptr ); };
template< typename TYPE, typename NAME, typename ... ARG >
struct call_static_function
{
	decltype( TYPE::template call_static_function< NAME, TYPE >( misc::construct< ARG >( ) ... ) )
	operator ( )( const ARG & ... r )
	{ return TYPE::template call_static_function< NAME, TYPE >( r ... ); }
};
template< typename TYPE, typename NAME >
struct call_static_function< TYPE, NAME, void >
{
	decltype( TYPE::template call_static_function< NAME, TYPE >( ) )
	operator ( )( )
	{ return TYPE::template call_static_function< NAME, TYPE >( ); }
};
#endif //STATIC_FUNCTION_HPP
