#include <iostream>

#include <so_5/all.hpp>
#include <so_5_extra/sync/pub.hpp>

using namespace std::chrono_literals;

class fixed_stack final : public so_5::agent_t {
  state_t st_empty{ this },
          st_filled{ this },
          st_full{ this };
 
  const size_t m_max_size;
  std::vector< int > m_stack;
 
public :
  struct push final { int m_val; };

  struct value final { int m_val; };
  struct stack_empty final {};

  using pop_reply = std::variant<value, stack_empty>;

  struct pop final {};
  using pop_request = so_5::extra::sync::request_reply_t<pop, pop_reply>;
 
  fixed_stack( context_t ctx, size_t max_size )
    : so_5::agent_t( ctx )
    , m_max_size( max_size )
  {
    this >>= st_empty;
 
    so_subscribe_self()
      .in( st_empty )
      .in( st_filled )
      .event( &fixed_stack::on_push );
 
    so_subscribe_self()
      .in( st_filled )
      .in( st_full )
      .event( &fixed_stack::on_pop_when_not_empty );
 
    so_subscribe_self()
      .in( st_empty )
      .event( &fixed_stack::on_pop_when_empty );
  }

private :
  void on_push( const push & w ) {
    m_stack.push_back( w.m_val );
    so_change_state( m_stack.size() == m_max_size ? st_full : st_filled );
  }
 
  void on_pop_when_not_empty( typename pop_request::request_mhood_t cmd ) {
    auto r = m_stack.back();
    m_stack.pop_back();
    so_change_state( m_stack.empty() ? st_empty : st_filled );
    cmd->make_reply( value{r} );
  }
 
  void on_pop_when_empty( typename pop_request::request_mhood_t cmd ) {
    cmd->make_reply( stack_empty{} );
  }
};  

int main() {
  so_5::launch( []( so_5::environment_t & env ) {
    so_5::mbox_t stack = env.introduce_coop( []( so_5::coop_t & coop ) {
        return coop.make_agent<fixed_stack>( 5u )->so_direct_mbox();
    } );

    // Fill stack.
    for( int i = 0; i < 10; ++i )
      so_5::send< fixed_stack::push >( stack, i );

    // Drain stack.
    std::cout << "stack { ";
    for(;;) {
      const auto r = fixed_stack::pop_request::ask_value( stack, 10s );
      if( auto * v = std::get_if<fixed_stack::value>( &r ) )
        std::cout << v->m_val << " ";
      else
        break;
    }
    std::cout << "}" << std::endl;

    env.stop();
  } );

  return 0;
}

// vim:fenc=utf8:ts=2:sts=2:sw=2:expandtab

