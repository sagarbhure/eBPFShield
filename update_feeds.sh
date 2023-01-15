#!/bin/bash

outdir="ip_feeds"
curl -vL "https://check.torproject.org/cgi-bin/TorBulkExitList.py?ip=1.1.1.1" > "$outdir/tor_exit_nodes.txt"
curl -vL "http://www.talosintelligence.com/documents/ip-blacklist" > "$outdir/talos_blacklist.txt"
