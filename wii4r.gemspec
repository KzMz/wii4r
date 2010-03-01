require 'rubygems'

GEM_SPEC = Gem::Specification.new do |spec|
	spec.name = "wii4r"
	spec.authors = ['KzMz', 'BuZz']
	spec.email = ['KzMz@modusbibendi.org', 'gambino.giorgio@gmail.com']
	spec.version = "0.5.0"
	spec.platform = Gem::Platform::RUBY
	
	spec.summary = "bindings for wiiuse, a C library for controlling Wiimotes"
	spec.description = "Ruby C Extension for controlling Wii Remote devices via bluetooth connection. Binding of C wiiuse library (www.wiiuse.net) "
	spec.required_ruby_version = ">= 1.8.6"
	spec.required_rubygems_version = ">= 1.3.5"
	
	spec.add_dependency "rake", ">= 0.8.3", "< 0.9"
	spec.add_dependency "rake-compiler", ">= 0.7.0"
	spec.require_path = "lib"
	spec.files = FileList["LICENSE", "Rakefile", "README.rdoc", "wii4r.gemspec", "examples/*.rb", "lib/*.*", "ext/**/*.{c, h, rb}"]
	
	spec.has_rdoc = true
	spec.rdoc_options << "--main" << "ext/wii4r/wii4r.c"
	spec.extra_rdoc_files = ['ext/wii4r/wii4r.c', "ext/wii4r/wiimotemanager.c", "ext/wii4r/wiimote.c", "ext/wii4r/nunchuk.c", "ext/wii4r/classic.c", "ext/wii4r/guitarhero3.c"]
	
	spec.homepage = "http://github.com/KzMz/wii4r"
	spec.licenses = ['GPL']
end


