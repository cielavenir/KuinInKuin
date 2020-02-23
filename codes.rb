#!/usr/bin/ruby
def snake2camel(s)
	a=s.split('_')
	r=a[0].downcase
	(1...a.size).each{|i|r<<a[i].capitalize}
	r
end
def process(s)
	s=(s.strip+"\n").inspect.gsub(/\\\\\{/){"\\{"}
	#s=s.gsub('Char','char16_t')
	s=s.gsub('S64','int64_t')
	s=s.gsub(/THROW\((.+)\)/){'throw \{excpt@'+snake2camel($1[6..-1])+' $ bit32}U'}
end

fname = ARGV.shift
funcname = File.basename(fname).split('.')[0]
puts '	case "'+funcname+'"'

ARGV.each_with_index{|e,i|
	if i==0
		puts '		do args.head()'
	else
		puts '		do args.next()'
	end
	puts '		var '+e+': \cpp\output@CppInfo :: \cpp\output@getInfo(args.get())'
}

File.open(fname).each_line{|s|
	puts '		do codes.add('+process(s)+')'
}
