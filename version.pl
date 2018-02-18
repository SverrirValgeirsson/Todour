#!/usr/bin/perl

use POSIX qw/strftime/;
use File::Copy qw(copy);


@git = `git tag`;
$version=pop @git;
#$stamp = strftime('%Y%m%d%H%M',localtime);
#$stamp = `date "+%Y%m%d%H%M%S"`;
chomp($version);
$version =~ s/v//;
$returned_version = $version;#.".".$stamp;
print $returned_version;

# make the versioning file in php
open(my $fh, '>', 'todour-latest_mac.php');
print $fh "";
print $fh "<?php echo \"".$returned_version."\" ?>";
close $fh;

copy 'todour-latest_mac.php', 'todour-latest_windows.php';
copy 'todour-latest_mac.php', 'todour-latest.php'; #used by Linux and older versions of win and mac



