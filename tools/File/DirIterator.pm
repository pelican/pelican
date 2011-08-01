# -----------------------------------------------
# File::DirIterator
# -----------------------------------------------
# Description: 
# Iterate through the contents of a directory and sub-directories
#
#
# -----------------------------------------------
# Copyright Chris Williams 2003
# -----------------------------------------------
# Interface
# ---------
# new(dir) : instantiate an iterator on the specified directory
# reset()  : reset the iterator
# base()   : return the directory to which the iterator refers
# next()   : return the next file, return undef when complete
# includeDirs() : set the iterator to return directories as well as files
# relativePath() : return the directory relative to the base dir, rather than the default full path

package File::DirIterator;
use strict;
use DirHandle;
1;

# -- initialisation

sub new {
    my $class=shift;

    my $self={};
    $self->{dir}=shift;
    bless $self, $class;
    $self->{includDir}=0;
    $self->{relativePath}=0;
    $self->reset();

    return $self;
}

sub reset {
    my $self=shift;
    @{$self->{arrayStack}} = [ $self->_readDir($self->{dir}) ];
    @{$self->{dirStack}}=();
    $self->{cdir}=$self->{dir};
}

sub relativePath {
    my $self=shift;
    $self->{relativePath}=1;
}

sub includeDirs {
    my $self=shift;
    $self->{includDir}=1;
}

sub base {
    my $self=shift;
    return $self->{dir};
}

sub next {
    my $self=shift;
    my $rv;

    while( ! defined $rv && $#{$self->{arrayStack}} >= 0) {
    #  ( ($#{$self->{curArray}} >= 0) || ($#{$self->{dirStack}} >= 0 )) ) {
        my $d=shift @{$self->{arrayStack}[0]};
        if( ! defined $d ) {
            $self->{cdir}= pop @{$self->{dirStack}};
            shift @{$self->{arrayStack}};
            next;
        }
        my $file=$self->{cdir}."/".$d;
        if( -d $file )
        {
            unshift @{$self->{arrayStack}}, [ $self->_readDir($file) ];
            push @{$self->{dirStack}}, $self->{cdir};
            $self->{cdir}=$file;
            $rv=$file, if( $self->{includDir} );
        }
        else {
            $rv=$file;
        }
    }
    if( $self->{relativePath} && defined $rv ) {
        my $base=$self->{dir};
        $rv=~s/^$base[\/\\]//;
    }
    return $rv;
}


# -- private methods -------------------------
sub _readDir {
    my $self=shift;
    my $dir=shift;
    my $dh =DirHandle->new($dir);
    return(), if ( !defined $dh );
    my @files=readdir($dh);
    return grep( !/^\.\.?/, @files);
}
