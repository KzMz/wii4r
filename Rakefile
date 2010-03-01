require 'rake/extensiontask'

load "wii4r.gemspec"

Rake::GemPackageTask.new(GEM_SPEC) do |pkg|
	pkg.need_zip = false
	pkg.need_tar = false
end

Rake::ExtensionTask.new("wii4r", GEM_SPEC)
