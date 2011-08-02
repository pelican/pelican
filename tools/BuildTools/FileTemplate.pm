# ----------------------------------
# class FileTemplate
# Description:
#    Generate a file from a template file
#-----------------------------------
# Methods:
# new(templateFile) :
# generateFile(filename, Environment )
#-----------------------------------

package BuildTools::FileTemplate;
use BuildTools::Environment;
use FileHandle;
use strict;
1;

sub new {
    my $class=shift;
    my $self={};
    $self->{template}=shift;
    bless $self, $class;
    return $self;
}

sub generateFile {
    my $self=shift;
    my $filename = shift;
    my $env=shift;
    if( ! -e $filename ) {
        my $fh = FileHandle->new(">".$filename) or die ( "unable to open $filename for output" );
        my $fin = FileHandle->new("<".$self->{template}) or die ("unable to open template $self->{template}" );
        while( <$fin> ) {
            print $fh $env->expandString($_);
        }
    }
}
