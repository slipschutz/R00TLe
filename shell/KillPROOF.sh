#!/bin/bash

ps aux | grep `whoami` | grep proofserv | awk ' { print $2} ' | xargs kill -9

ps aux | grep `whoami` | grep proofserv 