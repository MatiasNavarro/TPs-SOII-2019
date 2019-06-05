#!/usr/bin/perl -T
 
## Load pragmas and modules
use strict;
use warnings;
use CGI;
 
# The next two lines are very important. Do not modify them
# if you do not understand what they do.
$CGI::POST_MAX = 102400;
$CGI::DISABLE_UPLOADS = 1; 
 
 
####################################
#### User Configuration Section ####
####################################
 
# The path to where the downloadable files are. 
# Prefereably this should be above the web root folder.
# my $path_to_files = '/home/user/downloads/';
my $path_to_files = '/var/www/html/files/';
 
# The path to the error log file
my $error_log     = '/home/user/downloads/logs/errors.txt';
 
# Option to log errors: 1 = yes, 0 = no
my $log           = 0;
 
####################################
## End User Configuration Section ##
####################################
 
# Edit below here at your own risk
 
my $q = CGI->new;
 
######################################
## This section checks for a number ##
## of possible errors or suspicious ##
## activity.                        ##
######################################
 
# check to see if data limit is exceeded
if (my $error = $q->cgi_error()){
   if ($error =~ /^413\b/o) {
      error('Maximum data limit exceeded.');
   }
   else {
      error('An unknown error has occured.'); 
   }
}
 
# Check to see if the content-type is acceptable.
# multipart/form-data indicates someone is trying
# to upload data to the script with a hacked form.
# $CGI_DISABLE_UPLOADS prevents uploads. This routine
# is to catch the attempt and log it. 
if ($ENV{'CONTENT_TYPE'} =~ m|^multipart/form-data|io ) {
   error('Invalid Content-Type : multipart/form-data.')
}       
 
################################
## End error checking section ##
################################
 
# Get the data sent to the script.
my %IN = $q->Vars;
 
# Parse the "file" paramater sent to the script.
my $file = $IN{'file'} or error('No file selected.');
 
# Here we untaint the filename and make sure there are no characters like '/' 
# in the name that could be used to download files from any folder on the website.
if ($file =~ /^(\w+[\w.-]+\.\w+)$/o) {
   $file = $1;
}
else {
   error('Invalid characters in filename.');
}    
 
# Check if the download succeeded
download($file) or error('An unknown error has occured.');
 
 
#################
## SUBROUTINES ##
#################
 
# download the file
sub download {
   my $file = $_[0] or return(0);
 
   # Comment the next line if you uncomment the above line 
   open(my $DLFILE, '<', "$path_to_files/$file") or return(0);
 
   # This prints the download headers with the file size included
   # so you get a progress bar in the dialog box that displays during file downlaods. 
   print $q->header(-type            => 'application/x-download',
                    -attachment      => $file,
                    'Content-length' => -s "$path_to_files/$file",
   );
 
   #binmode $DLFILE;
   print while <$DLFILE>;
   undef ($DLFILE);
   return(1);
}
 
# This is a very generic error page. You should make a better one.
sub error {
   print $q->header(-type=>'text/html'),
         $q->start_html(-title=>'Error'),
         $q->h3("Error: $_[0]"),
         $q->end_html;
   log_error($_[0]) if $log;
   exit(0);
}
 
# Log the error to a file
sub log_error {
   my $error = $_[0];
 
   # Comment the next line if you uncomment the above line
   open (my $log, ">>", $error_log) or return(0);
 
   flock $log,2;
   my $params = join(':::', map{"$_=$IN{$_}"} keys %IN) || 'no params';
   print $log '"', join('","',time, 
                      scalar localtime(),
                      $ENV{'REMOTE_ADDR'},
                      $ENV{'SERVER_NAME'},
                      $ENV{'HTTP_HOST'},
                      $ENV{'HTTP_REFERER'},
                      $ENV{'HTTP_USER_AGENT'},
                      $ENV{'SCRIPT_NAME'},
                      $ENV{'REQUEST_METHOD'},
                      $params,
                      $error),
                      "\"\n";
}
