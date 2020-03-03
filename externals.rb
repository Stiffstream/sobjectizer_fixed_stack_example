MxxRu::arch_externals :so5 do |e|
  e.url 'https://github.com/Stiffstream/sobjectizer/archive/v.5.7.0.tar.gz'

  e.map_dir 'dev/so_5' => 'dev'
end

MxxRu::arch_externals :so5extra do |e|
  e.url 'https://github.com/Stiffstream/so5extra/archive/v.1.4.0.tar.gz'

  e.map_dir 'dev/so_5_extra' => 'dev'
end
