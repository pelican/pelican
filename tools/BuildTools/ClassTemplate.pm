# ----------------------------------
# class BuildTools::ClassTemplate
# Description:
#    Read Class Template secription files
# for C++
#-----------------------------------
# Methods:
# new() :
#-----------------------------------

package BuildTools::ClassTemplate;
use strict;
1;

sub new {
    my $class=shift;
    my $self={};
    $self->{file}=shift;
    bless $self, $class;
    return $self;
}

sub inherits {
    my $self=shift;
    return $self->{config}->list("inherits");
}

sub readFile {
    my $self=shift;
    
}

sub writeHeader {
    my $self=shift;
    my $fh=shift;
    my $classname=shift;
    my $filename=shift;
    my $indents=shift;
    my @namespaces=@_;

    my $i=" " x $indents;
    my $defname=$classname."_H";
    $defname=~tr/a-z/A-Z/;
    print $fh "#ifndef ", $defname, "\n",
              "#define $defname\n\n";

    foreach my $include ( $self->{config}->list("header::includes") ) {
        print $include,"\n";
    }
    foreach my $namespace ( @namespaces ) {
        print $fh "namespace $namespace {\n\n", if ( defined $namespace );
    }
    print $fh "/**\n",
              " * \@class $classname\n",
              " *  \n",
              " * \@brief\n",
              " * \n",
              " * \@details\n",
              " * \n",
              " */\n\n";
    my @baseclasses = $self->inherits();
    print $fh "class $classname";
    if( $#baseclasses >= 0 ) {
        my $sep=" :";
        foreach my $baseclass ( @baseclasses ) {
            print $fh $sep." public $baseclass";
            $sep=", ";
        }
    }
    print $fh  "\n{\n";
    print $fh  join("\n$i", $self->{config}->list("header::text"));
               $i,"public:\n",
               $i x 2, $classname, "( $constructHeadSig );\n",
               $i x 2,"~",$classname, "();\n",
               "\n";
               foreach my $method ( $self->implementSignatures() ) {
                   print $fh $i x 2,$method,";\n";
               }
               print $fh $i,"private:\n",
              "};\n\n";
    foreach my $namespace ( reverse @namespaces ) {
        print $fh "} // namespace $namespace\n", if ( defined $namespace );
    }
    print $fh "#endif // $defname \n";
    
}

sub writeBody {
    my $self=shift;
    my $fh=shift;
    my $classname=shift;
    my $filename=shift;
    my $header=shift;
    my @namespaces=@_;

    print $fh "#include \"", $header, "\"\n\n";
    foreach my $include ( $self->{config}->list("body::includes") ) {
        print $include,"\n";
    }
    foreach my $namespace ( @namespaces ) {
        print $fh "namespace $namespace {\n\n", if ( defined $namespace );
    }

    # constructor template
    print $fh "/**\n";
    print $fh " *\@details $classname \n";
    print $fh " */\n";
    print $fh $classname,"::", $classname, "($constructSig)\n";
    my @baseclasses = $self->inherits();
    if( $#baseclasses >= 0 ) { 
        my $sep="    : ";
        foreach my $baseclass ( @baseclasses ) {
            print $fh $sep.$baseclass."()" ),
            $sep=", ";
        }
        print $fh "\n";
    }

    # destructor
    print $fh "{\n",
              "}\n\n",
              "/**\n",
              " *\@details\n",
              " */\n",
              $classname, "::~", $classname, "()\n{\n",
              "}\n\n",
              $cpptext;

    foreach my $namespace (reverse @namespaces) {
        print $fh "} // namespace $namespace\n", if ( defined $namespace );
    }

    # pure virtual method sigs (i.e required to implement
    foreach my $method ( $self->implementSignatures() ) {
        print $fh "/**\n", " *\@details\n", " */\n",
                  $classname,"::";
        print $fh $method, "\n{\n}\n\n";
    }
    
}
