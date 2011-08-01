# ----------------------------------------
#
# Unit test for the FileIterator Class
#
# ----------------------------------------
#


package test_DirIterator;
use File::DirIterator;
use File::Sync qw(sync);
use FileHandle;
use strict;
1;

sub new {
    my $class=shift;
    my $self={};
    bless $self,$class;
    $self->{testConfigDir}=shift;
    $self->{tmpdir}=shift;
    return $self;
}

sub tests
{
    return qw(test_nonExistingDir test_emptyDir test_iterate test_relative);
}

sub test_nonExistingDir {
    my $self=shift;
    my $dir=$self->{tmpdir}."/IdoNotExist";
    my $it=File::DirIterator->new($dir);
    die "expecting undefined", if ( defined $it->next() );
}

sub test_emptyDir {
    my $self=shift;
    my $dir=$self->{tmpdir}."/IamEmpty";
    mkdir $dir or die "unable to create $dir :$!";
    my $it=File::DirIterator->new($dir);
    die "expecting undefined", if ( defined $it->next() );
}

sub test_iterate {
    my $self=shift;

    my @files;
    my $dir=$self->{tmpdir}."/IamFull";
    mkdir $dir or die "unable to create $dir :$!";
    push @files, $self->_touch($dir."/somefile1");
    push @files, $self->_touch($dir."/somefile2");
    my $dir2=$self->{tmpdir}."/IamFull/dir2";
    mkdir $dir2  or die "unable to create $dir2 :$!";
    push @files, $self->_touch($dir2."/testfile1");
    push @files, $self->_touch($dir2."/testfile2");
    my $dir3=$self->{tmpdir}."/IamFull/EmptyDir";
    mkdir $dir3  or die "unable to create $dir3 :$!";
    @files=sort( @files );

    sync();
    my $it=File::DirIterator->new($dir);
    $self->_testCommon($it, @files );

    my $it2=File::DirIterator->new($dir);
    $it2->includeDirs();
    $self->_testCommon($it2, sort( $dir2, $dir3, @files ));
}

sub test_relative {
    my $self=shift;

    my @files;
    my $dir=$self->{tmpdir}."/IamFull/relative";
    mkdir $dir or die "unable to create $dir :$!";
    $self->_touch($dir."/somefile1");
    $self->_touch($dir."/somefile2");
    my $dir2=$self->{tmpdir}."/IamFull/relative/dir2";
    mkdir $dir2  or die "unable to create $dir2 :$!";
    my @files=sort( qw(somefile1 somefile2 dir2) );

    sync();
    my $it=File::DirIterator->new($dir);
    $it->includeDirs();
    $it->relativePath();
    $self->_testCommon($it, @files );
}

sub _testCommon {
    my $self=shift;
    my $it=shift;
    my @files=@_;

    my @rec=();
    my $count=-1;
    my $i;
    while( defined ($i=$it->next()) ) {
        push @rec, $i;
        $count++;
        if( $count > $#files ) {
            die "more files returned than expected : got @rec";
        }
    }
    die "not enough files found got : @rec", if ( $#rec != $#files );
    @rec=sort(@rec);
    foreach my $f ( @rec )
    {
        die ( "unexpected file $f - expecting $files[0]" ), if $f ne (shift @files);
    }   
    return @rec;
}

sub _touch {
    my $self=shift;
    my $file=shift;
    my $fh=FileHandle->new(">".$file) or die("error creating file $file : $!");
    print $fh "Filename: $file\n";
    $fh->close();
    return $file;
}
