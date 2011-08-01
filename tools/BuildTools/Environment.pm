# ----------------------------------
# class Environment
# Description:
#   A convenience class to copy and merge hashes representing environment variables
#-----------------------------------
# Methods:
# new(@list_of_hashref) :
# namespace(name) : set/return the namespace of the variables
# deleteVar(name) : remove the named variable from the environment
# diff(Environment) : returns a new Environment containing values in this
#                     object that differ from the one passed
# env() : return a hash ref of the environment
# expandString(string) : expand the variables in the passed string
#                        marked with curly brackets e.g. ${varname} 
#                        use $$ to escape the $ identifier
# merge(hashref|Environment) : merge in the data, exisitng variables are not overriden
# remove(hashref|Environment) : remove the variables specified in the provided Environement
# set(var,value) : set a single variable
# size() : return the number of variables
# var(name) : return the value of the specified variable
# dump() : output to filestream
#-----------------------------------

package BuildTools::Environment;
use strict;
1;

sub new {
    my $class=shift;
    my $self={};
    bless $self, $class;
    $self->{env}={};
    for(@_) {
        $self->merge($_);
    }
    @{$self->{ns}}=("");
    return $self;
}

sub namespace {
    my $self=shift;
    if(@_) {
       @{$self->{ns}}=();
       for(@_){
           if($_ eq "" ) {
               push @{$self->{ns}},"";
           }
           else {
               push @{$self->{ns}},($_=~/.+::$/?$_:$_."::");
           }
       }
    }
    return (@{$self->{ns}});
}

sub env {
    my $self=shift;
    return $self->{env};
}

sub add {
    my $self=shift;
    push @{$self->{environments}}, @_;
}

sub var {
    my $self=shift;
    my $name=shift;
    return $self->expandString($self->{env}{$name},"");
}

sub set {
    my $self=shift;
    my $name=shift;
    $self->{env}{$name}=shift;
}

sub expandString {
    my $self=shift;
    my $string=shift;

    my @ns;
    if(@_) {
        @ns=@_;
    }
    else {
        @ns=@{$self->{ns}};
    }
    if( defined $string ) {
        foreach my $v ( keys %{$self->{env}} ) {
            foreach my $namesp ( @ns ) {
        #print "expandString() : env value : $v , namespace: $namesp\n";
                while( $string=~/(.*?)(?<!\$)\$\{\Q$namesp$v\E\}(.*(\n?))/g ) {
                    my $f1=$1;
                    my $f2=$2; 
                    $string=$f1.($self->expandString($self->{env}{$v},"")).$f2;
                }
                #$string=~s/(.*?)(?<!\$)\$\{$namesp$v\}(.*?)/$1$self->{env}{$v}$2/g;
            }
        }
        # -- send through other environments
        foreach my $e ( @{$self->{environments}} ) {
            $string=$e->expandString($string);
        }
    }
    return $string;
}

sub deleteVar {
    my $self=shift;
    my $var=shift;
    delete $self->{env}{$var};
}

sub merge {
    my $self=shift;

    foreach my $hash ( @_ ) {
        if( defined $hash ) {
            if( ref($hash) eq "Environment" ) {
                $hash=$hash->{env};
            }
            for( keys %$hash ) {
                $self->{env}{$_}=$hash->{$_}, if( ! defined $self->{env}{$_} );
            }
        }
    }
}

sub size {
    my $self=shift;
    my $size=scalar keys %{$self->{env}};
    return $size;
}

# returns a new Environment containing values in this
# object that differ from the one passed
sub diff {
    my $self=shift;
    my $env=shift;

    my $new={};
    for( keys %{$self->{env}} ) {
        my $r=$env->var($_);
        if( ! defined $r || $self->var($_) ne $r ) {
            $new->{$_}=$self->{env}{$_};
        }
    }
    return Environment->new($new);
}

sub remove {
    my $self=shift;
    my $hash=shift;
    if( defined $hash ) {
        if( ref($hash) eq "Environment" ) {
            $hash=$hash->{env};
        }
        foreach my $key( keys %$hash ) {
             delete $self->{env}{$key};
        }
    }
}

sub dump {
    my $self=shift;
    my $fh=shift;
    my $sep=shift;
    $sep="=", if( !defined $sep);
    for( keys %{$self->{env}} ) {
        print $fh $_.$sep.$self->var($_),"\n";
    }
}
