#!/usr/bin/ruby
require 'rubygems'

gem 'Mxx_ru', '>= 1.3.0'

require 'mxx_ru/cpp'

MxxRu::Cpp::exe_target {

  target 'fixed_stack_app'

  required_prj 'so_5/prj_s.rb'

  cpp_source 'main.cpp'
}

