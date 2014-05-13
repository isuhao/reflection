#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP
#include <memory>
#include "declare.hpp"
#include "any.hpp"
#include "../misc/misc.hpp"
#define EXAMPLE_NAME_SEQ (data)(cache)(func)(function)(foo)(bar)(test)
DECLARE_NAMES( EXAMPLE_NAME_SEQ )
struct test
{
	DECLARE_TYPE( test, EXAMPLE_NAME_SEQ );
	int data = 12450;
	double func( int ) const { return 10; }
	static int function( ) { return 1; }
	static int cache;
	static decltype( & function ) bar( long, long time ) { if ( time == 0 ) { } return & function; }
};
int test::cache = 0;
DECLARE_ANY( test, EXAMPLE_NAME_SEQ )
static_assert( has_member_function< test, tag< func >, long >::value, "" );
static_assert( ! has_member_function< test, tag< func >, void * >::value , "" );
static_assert( std::is_same< typename member_function_return_type< test, tag< func >, double >::type, double >::value, "" );
static_assert( ! has_static_function< test, tag< func >, void  >::value, "" );
static_assert( has_static_function< test, tag< function > >::value, "" );
static_assert( ! has_static_function< test, tag< func > >::value, "" );
static_assert( std::is_same< static_function_return_type< test, tag< function >, void >::type, int >::value, "" );
static_assert( ! has_member_function< test, tag< foo >, void >::value, "" );
static_assert( ! has_static_function< test, tag< foo > >::value, "" );
static_assert( has_static_variable< test, tag< cache > >::value, "" );
static_assert( ! has_static_variable< test, tag< data > >::value, "" );
static_assert( has_static_variable< test, tag< cache > >::value, "" );
static_assert( std::is_same< static_variable_type< test, tag< cache > >::type, int >::value, "" );
static_assert( has_static_function< test, tag< bar >, int, int >::value, "" );
static_assert(
		std::is_same
		<
			decltype( call_static_function< test, tag< bar >, int, int >( )( 0, 1 )( ) ),
			int
		>::value, "" );
static_assert(
		std::is_same
		<
			decltype( call_member_function< test, tag< func >, long >( )( std::declval< test >( ), ( 1 ) ) ),
			double
		>::value, "" );
static_assert( has_member_variable< test, tag< data > >::value, "" );
static_assert( ! has_member_variable< test, tag< cache > >::value, "" );
static_assert( std::is_same< member_variable_type< test, tag< data > >::type, int >::value, "" );
#include <iostream>
void example( )
{
	test t;
	any_test tr( t );
	auto ii = misc::make_expansion( [](int i){ std::cout << i << std::endl; }, [](...){ std::cout << "Hello World" << std::endl; } );
	tr.get_member_variable< tag< data > >( ii );
	tr.get_static_variable< tag< cache > >( ii );
	assert( ! tr.has_member_variable< tag< cache > >( ) );
	assert( tr.has_static_variable< tag< cache > >( ) );
	tr.call_member_function< tag< func > >( 1, ii );
	tr.call_static_function< tag< function > >( ii );
	tr.call_member_function< tag< function > >( ii );
	assert( ( ! tr.has_static_function< tag< func >, int >( ) ) );
	assert( ( tr.has_member_function< tag< func >, int >( ) ) );
}
#endif //EXAMPLE_HPP
