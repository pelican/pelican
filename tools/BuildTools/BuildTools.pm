# ----------------------------------
# class BuildTools
# Description:
#   Front end to access BuildTools confguration
# and general functions
#-----------------------------------
# Methods:
# new(ConfigFilesDir) :
# template(name)      : return the named template
#-----------------------------------

package BuildTools::BuildTools;
use strict;
use FindBin;
use lib "$FindBin::Bin";
use File::SearchPath;
use BuildTools::FileTemplate;
use BuildTools::ClassTemplate;
1;

sub new {
    my $class=shift;
    my $self={};
    bless $self, $class;
    my $base=shift;
    if( !defined $base ) { $base=$FindBin::Bin; }
    $self->{base}=$base;
    $self->{templates}=new File::SearchPath($base."/templates");
    return $self;
}

sub template {
    my $self=shift;
    my $template=shift;
    return new BuildTools::FileTemplate($self->{templates}->find($template));
}

sub classTemplate {
    my $self=shift;
    my $template=shift;

    return new BuildTools::ClassTemplate($self->{templates}->find($template."/class"));
}

sub namespaces {
    my $self=shift;

    my @namespaces;
    my $namespaceFile=$self->{base}."/namespaces";
    if( -f $namespaceFile ) {
        my $fh=FileHandle->new("<".$namespaceFile);
        while( <$fh> ) {
            next, if( $_=~/^\s*#/ );
            next, if( $_=~/^\s*$/ );
            chomp;
            push @namespaces, $_;
        }
    }
    return @namespaces;
}
