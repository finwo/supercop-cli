<!-- Creator     : groff version 1.22.4 -->
<!-- CreationDate: Thu Sep 16 21:22:39 2021 -->
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta name="generator" content="groff -Thtml, see www.gnu.org">
<meta http-equiv="Content-Type" content="text/html; charset=US-ASCII">
<meta name="Content-Style" content="text/css">
<style type="text/css">
       p       { margin-top: 0; margin-bottom: 0; vertical-align: top }
       pre     { margin-top: 0; margin-bottom: 0; vertical-align: top }
       table   { margin-top: 0; margin-bottom: 0; vertical-align: top }
       h1      { text-align: center }
</style>
<title>supercop</title>

</head>
<body>

<h1 align="center">supercop</h1>

<a href="#NAME">NAME</a><br>
<a href="#SYNOPSIS">SYNOPSIS</a><br>
<a href="#DESCRIPTION">DESCRIPTION</a><br>
<a href="#OPERATIONS">OPERATIONS</a><br>
<a href="#OPTIONS">OPTIONS</a><br>
<a href="#RETURN VALUE">RETURN VALUE</a><br>
<a href="#ENVIRONMENT">ENVIRONMENT</a><br>
<a href="#AUTHOR">AUTHOR</a><br>

<hr>


<h2>NAME
<a name="NAME"></a>
</h2>


<p style="margin-left:11%; margin-top: 1em">supercop -
Simplified CLI for supercop-ref10</p>

<h2>SYNOPSIS
<a name="SYNOPSIS"></a>
</h2>



<p style="margin-left:11%; margin-top: 1em"><b>supercop</b>
<i>&lt;operation&gt;</i> [<b>-k</b> <i>keyfile</i>]
[<b>-m</b> <i>message</i>] [<b>-M</b> <i>message-file</i>]
[<b>-s</b> <i>signature</i>]</p>

<h2>DESCRIPTION
<a name="DESCRIPTION"></a>
</h2>


<p style="margin-left:11%; margin-top: 1em">This program
allows easy signing and signature verification for the
supercop-ref10 implementation of the ed25519 signature
scheme.</p>

<h2>OPERATIONS
<a name="OPERATIONS"></a>
</h2>



<p style="margin-left:11%; margin-top: 1em"><b>&minus;&minus;printkey</b></p>

<p style="margin-left:22%;">Print contents of key file</p>


<p style="margin-left:11%;"><b>&minus;&minus;generate</b></p>

<p style="margin-left:22%;">Generate a new key</p>

<table width="100%" border="0" rules="none" frame="void"
       cellspacing="0" cellpadding="0">
<tr valign="top" align="left">
<td width="11%"></td>
<td width="9%">


<p><b>&minus;&minus;sign</b></p></td>
<td width="2%"></td>
<td width="21%">


<p>Sign a message</p></td>
<td width="57%">
</td></tr>
</table>


<p style="margin-left:11%;"><b>&minus;&minus;verify</b></p>

<p style="margin-left:22%;">Verify a message signature</p>


<p style="margin-left:11%;"><b>&minus;&minus;version</b></p>

<p style="margin-left:22%;">Show version number and
exit</p>

<h2>OPTIONS
<a name="OPTIONS"></a>
</h2>


<p style="margin-left:11%; margin-top: 1em"><b>&minus;h,
&minus;&minus;help</b></p>

<p style="margin-left:22%;">Show usage and exit</p>

<p style="margin-left:11%;"><b>&minus;k,
&minus;&minus;key&minus;file</b> <i>path</i></p>

<p style="margin-left:22%;">Select key file to use for the
operation</p>

<p style="margin-left:11%;"><b>&minus;m,
&minus;&minus;message</b> <i>message_string</i></p>

<p style="margin-left:22%;">Message to sign or verify
(defaults to stdin)</p>

<p style="margin-left:11%;"><b>&minus;M,
&minus;&minus;message&minus;file</b> <i>path</i></p>

<p style="margin-left:22%;">Message file to sign or verify
(defaults to stdin)</p>

<p style="margin-left:11%;"><b>&minus;s,
&minus;&minus;signature</b> <i>signature_string</i></p>

<p style="margin-left:22%;">Signature to verify the message
against</p>

<h2>RETURN VALUE
<a name="RETURN VALUE"></a>
</h2>


<p style="margin-left:11%; margin-top: 1em">Here are the
possible return values:</p>

<table width="100%" border="0" rules="none" frame="void"
       cellspacing="0" cellpadding="0">
<tr valign="top" align="left">
<td width="22%"></td>
<td width="1%">


<p>&bull;</p></td>
<td width="2%"></td>
<td width="75%">


<p><b>0</b> Everything went fine or the signature was
valid</p> </td></tr>
<tr valign="top" align="left">
<td width="22%"></td>
<td width="1%">


<p>&bull;</p></td>
<td width="2%"></td>
<td width="75%">


<p><b>1</b> Signature was invalid or an error occurred, see
stderr for an explanation</p></td></tr>
</table>

<h2>ENVIRONMENT
<a name="ENVIRONMENT"></a>
</h2>


<p style="margin-left:11%; margin-top: 1em">supercop
doesn&rsquo;t follow any environment variables</p>

<h2>AUTHOR
<a name="AUTHOR"></a>
</h2>


<p style="margin-left:11%; margin-top: 1em">finwo
(https://finwo.nl)</p>
<hr>
</body>
</html>
