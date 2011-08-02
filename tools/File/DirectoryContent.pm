# ----------------------------------
# class File::DirectoryContent
# Description:
#   Object to access the contents of a directory
#-----------------------------------
# Methods:
# new(dirname) :
# exclude(@regexps) : a list of exclude patterns
# files() : a list of files in the directory (relative to dirname)
# dirs()  : a list of directories contained in the base dir ( relative to dirname)
# symlinks() : a hashref of links ( keys=link name(relative to dirname, value=destination)
# replicate(dir) : make a clone of the directory at the specified location
#-----------------------------------


package File::DirectoryContent;
use File::DirIterator;
use File::Copy;
use strict;
1;

sub new {
    my $class=shift;
    my $self={};
    bless $self, $class;
    $self->{dir}=shift;
    $self->exclude(@_);
    $self->{parsed}=0;
    return $self;
}

sub replicate {
    my $self=shift;
    my $dest=shift;
    $self->_parse();
    $self->_mkdir($dest);
    for( $self->dirs() ) {
        $self->_mkdir($dest.$_);
    }
    for( $self->files() ) {
        copy($self->{dir}.$_, $dest.$_) or die("unable to copy file $_ to $dest.$_ : $!");
        my $perm = (stat $self->{dir}.$_)[2] & 07777;
        chmod($perm | 0600, $dest.$_);
    }
#    my $links=$self->links();
#    foreach my $l ( keys %{$self->symlinks()} ) {
#        symlink($links->{$l}, $dest.$l ) or die("unable to create symlink $dest.$l : $!");
#    }
}

sub exclude {
    my $self=shift;
    push @{$self->{excludes}}, @_;
}

sub parse {
     my $self=shift;
     my $dir=$self->{dir};
     my @exclude=@{$self->{excludes}};

     my $it=File::DirIterator->new($dir);
     $it->relativePath();
     $it->includeDirs();
     my @files=();
     my @dirs=();
     my $symlinks={};
     my $file;
     while( defined ($file=$it->next()) ) {
         my $found=0;
         $file="/".$file;
         foreach my $exc ( @exclude ) {
             if( $file=~/^$exc/ ) {
                 $found=1;
                 last;
             }
         }
         if( ! $found ) {
             my $fl=$self->{dir}.$file;
             if( -l $fl ) {
                 $symlinks->{$file}=readlink($fl); 
             }
             elsif( -f $fl ) {
                 push (@files, $file); 
             }
             elsif( -d $fl ) {
                 push (@dirs, $file);
             }
         }
     }
     @{$self->{dirs}}=sort(@dirs);
     @{$self->{files}}=sort(@files);
     $self->{symlinks}=$symlinks;
}

sub dirs {
    my $self=shift;
    $self->_parse();
    return (@{$self->{dirs}});
}

sub files {
    my $self=shift;
    $self->_parse();
    return (@{$self->{files}});
}

sub symlinks {
    my $self=shift;
    $self->_parse();
    return $self->{symlinks};
}

sub _parse {
    my $self=shift;
    if( ! $self->{parsed} ) {
        $self->parse();
        $self->{parsed}=1;
    }
}

sub _mkdir {
    my $self=shift;
    my $dir=shift;
    if( ! -d $dir ) {
        mkdir $dir or die "unable to create directory $dir : $!";
    }
}
