#include <iostream>

#include <so_5/all.hpp>

class fixed_stack : public so_5::agent_t
{
  state_t st_empty{ this }, st_filled{ this }, st_full{ this };
 
  const size_t m_max_size;
  std::vector< int > m_stack;
 
public :
  class empty_stack : public std::logic_error
  {
  public :
    using std::logic_error::logic_error;
  };

  fixed_stack( context_t ctx, size_t max_size )
    : so_5::agent_t( ctx ), m_max_size( max_size )
    {}
 
  struct push { int m_val; };
  struct pop : public so_5::signal_t {};
 
  virtual void so_define_agent() override
  {
    this >>= st_empty;
 
    so_subscribe_self().in( st_empty ).in( st_filled )
      .event( [this]( const push & w ) {
          m_stack.push_back( w.m_val );
          this >>= ( m_stack.size() == m_max_size ? st_full : st_filled );
        } );
 
    so_subscribe_self().in( st_filled ).in( st_full )
      .event< pop >( [this]() -> int {
          auto r = m_stack.back();
          m_stack.pop_back();
          this >>= ( m_stack.empty() ? st_empty : st_filled );
          return r;
        } );
 
    so_subscribe_self().in( st_empty )
      .event< pop >( []() -> int { throw empty_stack( "empty_stack" ); } );
  }
};  

int main()
{
  try
  {
    so_5::launch( []( so_5::environment_t & env ) {
      so_5::mbox_t stack;
      env.introduce_coop( [&stack]( so_5::coop_t & coop ) {
        stack = coop.make_agent< fixed_stack >( 5 )->so_direct_mbox();
      } );

      for( int i = 0; i < 10; ++i )
        so_5::send< fixed_stack::push >( stack, i );

      std::cout << "stack { ";
      try
      {
        for(;;)
          std::cout << so_5::request_value< int, fixed_stack::pop >(
              stack, std::chrono::seconds(10) ) << " ";
      }
      catch( const fixed_stack::empty_stack & ) {}
      std::cout << "}" << std::endl;

      env.stop();
    } );
  }
  catch( const std::exception & x )
  {
    std::cerr << "Oops! " << x.what() << std::endl;
  }

  return 2;
}

// vim:fenc=utf8:ts=2:sts=2:sw=2:expandtab

