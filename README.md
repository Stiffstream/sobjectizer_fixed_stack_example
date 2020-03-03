# What is it?

This is an implementation of fixed_stack example from C++ Actor Framework distributive, but using SObjectizer-5.7 and so5extra.

# How to get and try?

To compile the example Ruby, RubyGems and Rake are required. Usually they are available from the single package, but sometimes it can be necessary to install them manually. For example:

```sh
sudo apt install ruby
sudo apt install rake
```
After installing of Ruby (+RubyGems+Rake) it is necessary to install Mxx_ru:
```sh
gem install Mxx_ru
```
Or, if root privilegies are required:
```sh
sudo gem install Mxx_ru
```

Now you can get the source code from GitHub and compile it:
```sh
# Get the sources.
git clone https://github.com/Stiffstream/sobjectizer_fixed_stack_example
cd sobjectizer_fixed_stack_example
# Get the dependencies.
mxxruexternals
# Compile.
cd dev
ruby build.rb
```
As result of the compilation there will be `fixed_stack_app` executable in `target/release` folder.
