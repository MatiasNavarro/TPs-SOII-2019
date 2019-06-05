#!/usr/bin/perl -wT
 
use strict;
use warnings;
use CGI;
use CGI::Carp qw/fatalsToBrowser/;
use File::Basename;
 
$CGI::POST_MAX = 1024 * 5000; #adjust as needed (1024 * 5000 = 5MB)
$CGI::DISABLE_UPLOADS = 0; #1 disables uploads, 0 enables uploads
 
my $query = CGI->new;
 
unless ($CGI::VERSION >= 2.47) { 
   error('Your version of CGI.pm is too old. You must have verison 2.47 or higher to use this script.')
}
 
my $upload_dir = '/var/www/html/upload';
 
# a list of valid characters that can be in filenames
my $filename_characters = 'a-zA-Z0-9_.-';
 
my $file = $query->param("modulo") or error('No file selected for upload.') ;

# get the filename and the file extension
# this could be used to filter out unwanted filetypes
# see the File::Basename documentation for details
my ($filename,undef,$ext) = fileparse($file,qr{\..*});

if ($ext ne ".ko") {
    error("Usted no ha ingresado un modulo correcto.");
} 

# append extension to filename
$filename .= $ext;
 
# convert spaces to underscores "_"
$filename =~ tr/ /_/;
 
# remove illegal characters
$filename =~ s/[^$filename_characters]//g;
 
# satisfy taint checking
if ($filename =~ /^([$filename_characters]+)$/) {
   $filename = $1;
}
else{
   error("The filename is not valid. Filenames can only contain these characters: $filename_characters")
}
 
my $upload_filehandle = $query->upload("modulo");
 
open (UPLOADFILE, ">$upload_dir/$filename") or error($!);
binmode UPLOADFILE;
while ( <$upload_filehandle> ) {
   print UPLOADFILE;
}
close UPLOADFILE;
 
my $output_cmd = system("sudo insmod /var/www/html/upload/$filename");

if ($output_cmd ne 0) {
   error('ERROR! No se pudo instalar el modulo seleccionado.');
}
else{
   print $query->header ( );
   print <<END_HTML;
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Thanks!</title>
<style type="text/css">
img {border: none;}
</style>
</head>
<body>
<p>Modulo '$filename' cargado correctamente</p>
</body>
</html>
END_HTML
   exit(0);
}
 
sub error {
   print $query->header(),
         $query->start_html(-title=>'Error'),
         shift,
         $query->end_html;
   exit(0);
}
