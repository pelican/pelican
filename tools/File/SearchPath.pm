# ----------------------------------
# class File::SearchPath
# Description:
#
#-----------------------------------
# Methods:
# new([path]) : optional colon seperated path list
# paths() : return a list of paths
# find("filename") : search the paths for the named file - returns all matching files in order
# add( @paths )  : add a list of paths
#-----------------------------------


package File::SearchPath;
use strict;
use FileHandle;
use Cwd;
1;

sub new {
    my $class=shift;
    my $self={};
    bless $self, $class;
    foreach my $path ( @_ ) {
        if( defined $path && $path ne "" ) {
            $self->add( split (/:/, $path) );
        }
    }
    return $self;
}

sub add {
    my $self=shift;
    foreach my $dir ( @_ ) {
        # expand relative paths
        my $tmp=$dir;
        if( $dir!~/^[\\\/].*/ ) {
            my $cwd=getcwd();
            $tmp=$cwd."/$dir";
        }
        # eliminate .. from the path
        while( $tmp=~/.+[\/\\]\.\./ ) {
            $tmp=~s/(.*[\\\/]).+?[\\\/]\.\.(.*)/$1$2/;
        }
        $tmp=~s/([\\\/]{2})/\//g; # remove double seperators
        $tmp=~s/(.+)[\\\/]$/$1/; # remove trailing /
        push @{$self->{paths}}, cleanPath($tmp);
    }
}

sub exists {
    my $self=shift;
    my $file=shift;

    my $rv=0;
    foreach my $dir ( @{$self->{paths}} ) {
        my $fnm=$dir."/".$file;
        if ( -f $fnm ) {
            $rv=1; last;
        }
    }
    return $rv;
}

sub find {
    my $self=shift;

    my @found;
    foreach my $dir ( @{$self->{paths}} ) {
        foreach my $file ( @_ ) {
            my $fnm=$dir."/".$file;
            if ( -f $fnm ) { push @found, $fnm; }
        }
    }
    return @found;
}

sub paths {
    my $self=shift;
    return @{$self->{paths}};
}

sub cleanPath {
    my $dir=shift;
    # expand relative paths
    if( $dir!~/^[\\\/].*/ ) {
        $dir=getcwd()."/".$dir;
    }
    # eliminate .. from the path
    while( $dir=~/.+[\/\\]\.\./ ) {
        $dir=~s/(.*[\\\/]).+?[\\\/]\.\.(.*)/$1$2/;
    }
    $dir=~s/([\\\/]{2})/\//g; # remove double seperators
    $dir=~s/(.+)[\\\/]$/$1/; # remove trailing /
    return $dir;
}
