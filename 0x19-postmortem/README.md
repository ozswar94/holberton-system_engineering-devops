# 0x19 Postmortem
## Issue Summary

10/06/2021 08h00 to 09h50.

The users contact the support team because they have an error message on the "500 Internal Server Error" website,
following which the support team creates a ticket and transfers the ticket to the admin team to solve the incident.
The problem was because of a config file, there was an error one line and this call has another file but the file name was badly written.

## Timeline

08h00 - ticket create by the support team

08h05 - the ticket is tranfered to admin team

08h20 - the admin team investigate

09h50 - the incident is resolve, it was an error in config file

## Root Cause
During the deployment of the night the sys admin change accidently the of the name of the file.
## Corrective and Preventative Measure
The file name has been changed. Now before every change of file config we check with a new tool create by the admin team.
