# -----------------------------------------------
# CMakeModules
# -----------------------------------------------
# Description: 
#    Maintain list of cmake modules
#
#
# -----------------------------------------------
# Copyright Chris Williams 2006 - 2009
# -----------------------------------------------
# Interface
# ---------
# new(file)    : new object
# update()     : update the file
#

package CMakeModules;
use strict;
use FileHandle;
use DirHandle;
use File::Basename;
use Cwd;
1;

# -- initialisation

sub new {
    my $class=shift;

    my $self={};
    bless $self, $class;
    $self->{file}=shift;
    if( $self->{file}!~/^[\\\/]/ ) {
        $self->{file}=cwd()."/".$self->{file};
    }
    $self->{dir}=dirname($self->{file});

    return $self;
}

sub update {
    my $self=shift;
    $self->_read($self->{file});
    # read in our directories
    my $fh=FileHandle->new(">>".$self->{file}) or die ("unable to write file ".$self->{file}." : $!");
    my $dh=DirHandle->new($self->{dir}) 
        or die("unable to read directory ".$self->{dir}." : $!");
    for($dh->read()) {
       next, if ($_=~/\.+/);
       my $dir=$self->{dir}."/".$_;
       next, if (! -d $dir );
       my $cm=$dir."/CMakeLists.txt";
       if( -f $cm ) {
           # this is a module
           if( ! defined $self->{modules}{$_} ) {
               $self->{modules}{$_}="add_subdirectory($_)\n";
               print $fh $self->{modules}{$_};
           }
       }
    }
}

# -- private methods -------------------------
sub _read {
    my $self=shift;
    my $file=shift;
    if( -f $file ) {
        my $fh=FileHandle->new("<".$file);
        while(<$fh>) {
            if($_=~/add_subdirectory\((.+)\)/) {
                $self->{modules}{$1}=$_;
            }
        }
    }
}
