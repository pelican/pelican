# ----------------------------------
# class test_DirectoryContent
# Description:
#
#-----------------------------------
# Methods:
# new() :
#-----------------------------------


package test_DirectoryContent;
use strict;
use FileHandle;
use File::DirectoryContent;
1;

sub new {
    my $class=shift;
    my $self={};
    bless $self, $class;
    $self->{testConfigDir}=shift;
    $self->{tmpdir}=shift;
    $self->_createStucture();
    return $self;
}

sub tests {
    return qw( test_full test_exclude test_excludeall );
}

sub test_full {
    my $self=shift;
    my $dir=$self->{tmpdir}."/a";
    my $cont=File::DirectoryContent->new($dir);
    my @files=$cont->files();
    my @expect=sort(qw(/file /filea /b/file /b/c/file /b/c/d/file /b/fileb /b/c/filec /b/c/d/filed));
    die("expecting @expect, get @files"), if( "@expect" ne "@files" );
    my @dirs=$cont->dirs();
    my @expectdirs=sort(qw(/b /b/c /b/c/d ));
    die("expecting @expect, get @files"), if( "@expect" ne "@files" );
}

sub test_exclude {
    my $self=shift;
    my $dir=$self->{tmpdir}."/a";
    my $exclude="/b/c/d";
    my $cont=File::DirectoryContent->new($dir);
    $cont->exclude($exclude);
    my @files=$cont->files();
    my @expect=sort(qw(/file /filea /b/file /b/fileb /b/c/file /b/c/filec));
    die("expecting @expect, get @files"), if( "@expect" ne "@files" );
}

sub test_excludeall {
    my $self=shift;
    my $dir=$self->{tmpdir}."/a";
    my $exclude="/";
    my $cont=File::DirectoryContent->new($dir);
    $cont->exclude($exclude);
    my @files=$cont->files();
    my @expect=();
    die("expecting @expect, get @files"), if( "@expect" ne "@files" );
}

sub _createStucture {
    my $self=shift;
    my $d=$self->{tmpdir};
    # make a/b/c/d hierarcy
    foreach my $dir ( qw(a b c d) ) {
        $d.="/".$dir;
        mkdir $d or die "unable to create dir $d\n";
        my $file=$d."/file"; # identically named file in each dir
        $self->_touch($file);
        my $ufile=$d."/file".$dir; # uniquely named file in each dir
        $self->_touch($ufile);
    }
}

sub _touch {
    my $self=shift;
    my $file=shift;
    my $fh=FileHandle->new(">".$file) or die("error creating file $file : $!");
    print $fh "Filename: $file\n";
    $fh->close();
    return $file;
}
