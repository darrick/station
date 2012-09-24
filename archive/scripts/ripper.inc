; These are the settings for the stream ripper script.

; Complete URL of webstream to rip
stream_url = "http://127.0.0.1:8000"

; Full path to the stream ripper executable, version 1.61.17 or higher.
streamripper_path = "/usr/bin/streamripper"

; Full path to station archive module's import directory (no trailing slash)
import_path = "/var/www/drupal7/sites/all/modules/station/archive/import"

; Because shows tend to start early or finish late, the overlap_seconds
; setting allows you to determine how much the recording should overlap at the
; end of the hour. The suggestion is to schedule the script to start one minute
; before the hour and use 60 seconds of overlap. That will result in a 62 minute
; recording, with one minute before the hour and an extra minute at the end.
overlap_seconds = 60

; The file type of the stream; "mp3" and "ogg" are supported.
file_format = "mp3"

; Pre-rip command - command(s) to be executed before the ripping starts.
; Separate multiple commands with a semicolon. This is meant for shell commands,
; and won't parse PHP directly. 
prerip_command = ""

; Post-rip command - command(s) to be executed at the end of the ripping script.
; Separate multiple commands with a semicolon. This is meant for shell commands,
; and won't parse PHP directly.
postrip_command = ""
