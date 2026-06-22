cat << EOF
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<app version="1">
	<name>Korbo's Donut Shop</name>
	<version>$1</version>
	<release_date>$(date -u +%Y%m%d%H%M%S)</release_date>
	<!-- date format is YYYYMMDDhhmmss, UTC-->
	<coder>Korbosoft</coder>
	<short_description>Korbo's spinning donut!</short_description>
	<long_description>Korbo's world-famous* donut shop is always open!

*Donut shop might not actually be "world-famous".
None of this would have ever happened without "Wii Donut" by emilydaemon, and
xOx-Liy-Codez-XD-oXo helped me a ton getting this to initially work.

Music can be changed by adding a "music.(mod/it/s3m/xm/whatever)" file in the app directory.
This program supports *most* tracker music formats. https://modarchive.org is a good source.

e-mail:
Korbo:       korbin.m.marshall@gmail.com
	</long_description>
</app>
EOF
