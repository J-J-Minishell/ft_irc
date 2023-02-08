# FT_IRC

Simple IRC Server based on the RFC (mainly 2812) written in C++.

FT_IRC does not support server-server communication and it works with any IRC client that send packets terminated with `\r\n`.

# Run

Do `make` and then run :

```bash
./ft_irc <port> <password>
```

# Connect

If you'd like to try the server without using a IRC client, you can connect to it using the following steps.

- First open a new terminal and type:
```bash
nc -c <server host> <port>
```
- Once you are connected, you can register like a normal IRC server does.
```bash
PASS <password>
```
```bash
NICK <nickname>
```
```bash
USER <username> 0 * <realname>
```

- Then the server will send you a PING and you shoud answer with a PONG for register succesfully.
```bash
PONG server
```

- Now you can create channels, send private messages, etc.

## Handled commands

This following list of commands are handled on our server:

```
- PASS
- NICK
- USER
- PING
- PONG
- QUIT
- NOTICE
- PRIVMSG
- JOIN
- KICK
- LIST
- NAMES
- PART
- NICK
- OPER
- DIE
```

## Subject pdf
- https://cdn.intra.42.fr/pdf/pdf/32224/en.subject.pdf

## MADE BY :

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="https://github.com/baringo11/"><img src="https://avatars.githubusercontent.com/u/57132782?s=400&v=4" width="100px;" alt=""/><br /><sub><b>baringo11 (jbaringo)</b></sub></a><br /> <br /><a href="https://profile.intra.42.fr/users/jbaringo" title="Intra 42"><img src="https://img.shields.io/badge/Madrid-FFFFFF?style=plastic&logo=42&logoColor=000000" alt="Intra 42"/></a></td>
    <td align="center"><a href="https://github.com/jgalian/"><img src="https://avatars.githubusercontent.com/u/62511524?v=4" width="100px;" alt=""/><br /><sub><b>jgalian (jgalian-)</b></sub></a><br /> <br /><a href="https://profile.intra.42.fr/users/jgalian-" title="Intra 42"><img src="https://img.shields.io/badge/Madrid-FFFFFF?style=plastic&logo=42&logoColor=000000" alt="Intra 42"/></a></td>
    <td align="center"><a href="https://github.com/ccardozom/"><img src="https://avatars.githubusercontent.com/u/54796906?v=4" width="100px;" alt=""/><br /><sub><b>ccardozom (ccardozo)</b></sub></a><br /> <br /><a href="https://profile.intra.42.fr/users/ccardozo" title="Intra 42"><img src="https://img.shields.io/badge/Madrid-FFFFFF?style=plastic&logo=42&logoColor=000000" alt="Intra 42"/></a></td>
  </tr>
</table>
<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->
<!-- ALL-CONTRIBUTORS-LIST:END -->

