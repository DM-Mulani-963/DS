#!/usr/bin/env python3
"""Build the complete DS Interactive Hub HTML file."""
import os

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                   "Distributed_Systems_Interactive_Hub.html")

parts = []

# ── Part 1: HTML head + CSS ──
parts.append(r'''<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Distributed Systems: The Ultimate Learning Hub</title>
<style>
:root{--pri:#1a1a2e;--sec:#16213e;--acc:#3498db;--hl:#e94560;--lt:#f0f2f5;--tx:#333;--cb:#fff;--cbd:#e0e0e0;--ok:#27ae60;--err:#e74c3c;--wrn:#f39c12;--code:#1e1e2e}
[data-theme=dark]{--pri:#0d1117;--sec:#161b22;--acc:#58a6ff;--hl:#f85149;--lt:#0d1117;--tx:#c9d1d9;--cb:#161b22;--cbd:#30363d;--code:#0d1117}
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:"Segoe UI",system-ui,sans-serif;display:flex;height:100vh;background:var(--lt);color:var(--tx);overflow:hidden;transition:background .3s,color .3s}
#sidebar{width:300px;min-width:300px;background:var(--pri);color:#ccd6f6;display:flex;flex-direction:column;box-shadow:2px 0 10px rgba(0,0,0,.3)}
#sidebar header{padding:20px;background:var(--sec);text-align:center;font-size:1.2rem;font-weight:bold;color:#fff;border-bottom:3px solid var(--hl)}
.sc{display:flex;gap:6px;padding:8px 12px;background:rgba(0,0,0,.2)}
.sc button{flex:1;background:rgba(255,255,255,.08);border:1px solid rgba(255,255,255,.15);color:#fff;padding:5px;font-size:.72rem;border-radius:4px;cursor:pointer}
.sc button:hover{background:rgba(255,255,255,.18)}
#pb{height:3px;background:rgba(255,255,255,.1)}
#pf{height:100%;background:var(--hl);transition:width .4s;width:0%}
#nl{flex:1;overflow-y:auto;list-style:none}
#nl::-webkit-scrollbar{width:4px}
#nl::-webkit-scrollbar-thumb{background:rgba(255,255,255,.2);border-radius:2px}
.ni{padding:12px 18px;cursor:pointer;border-bottom:1px solid rgba(255,255,255,.04);transition:all .2s;font-size:.85rem;display:flex;align-items:center}
.ni:hover{background:rgba(255,255,255,.08)}
.ni.active{background:var(--hl);color:#fff;font-weight:600}
.ni .ic{margin-right:10px;font-size:.75rem;opacity:.6}
.ni.visited::after{content:"done";margin-left:auto;font-size:.6rem;background:var(--ok);color:#fff;padding:1px 6px;border-radius:8px}
#content{flex:1;overflow-y:auto;padding:35px 40px;display:flex;flex-direction:column}
.sec{display:none;animation:fi .4s}.sec.active{display:block}
@keyframes fi{from{opacity:0;transform:translateY(8px)}to{opacity:1;transform:translateY(0)}}
h1{margin-bottom:15px;color:var(--pri);border-bottom:3px solid var(--hl);padding-bottom:8px;display:inline-block;font-size:1.7rem}
[data-theme=dark] h1{color:#fff}
h2{margin:22px 0 10px;color:var(--acc);font-size:1.25rem}
h3{margin:15px 0 8px;font-size:1.05rem}
p{line-height:1.7;margin-bottom:12px}
ul,ol{padding-left:22px;margin-bottom:12px}
li{line-height:1.6;margin-bottom:4px}
code{background:rgba(0,0,0,.06);padding:2px 6px;border-radius:3px;font-size:.88em}
[data-theme=dark] code{background:rgba(255,255,255,.1)}
pre{background:var(--code);color:#d4d4d4;padding:16px;border-radius:8px;overflow-x:auto;font-size:.82rem;line-height:1.5;margin:10px 0}
.dc{background:var(--cb);border:1px solid var(--cbd);border-radius:12px;padding:25px;margin:20px 0;box-shadow:0 2px 15px rgba(0,0,0,.04)}
.cv{width:100%;height:350px;background:var(--lt);border:1px dashed var(--cbd);border-radius:8px;position:relative;overflow:hidden;margin:12px 0}
.nd{width:55px;height:55px;background:var(--acc);color:#fff;border-radius:50%;display:flex;flex-direction:column;align-items:center;justify-content:center;position:absolute;font-weight:bold;cursor:pointer;box-shadow:0 3px 10px rgba(0,0,0,.2);transition:transform .2s;font-size:.8rem}
.nd:hover{transform:scale(1.1)}
.nd.leader{border:3px solid var(--hl);background:#f1c40f;color:#333}
.nd.dead{background:#666!important;opacity:.5}
.nd.rec{box-shadow:0 0 0 4px rgba(231,76,60,.5)}
.mg{width:14px;height:14px;background:var(--hl);border-radius:50%;position:absolute;z-index:10}
.ct{display:flex;gap:8px;margin-bottom:12px;flex-wrap:wrap}
button{padding:9px 18px;border:none;border-radius:6px;background:var(--acc);color:#fff;cursor:pointer;font-weight:600;font-size:.85rem;transition:filter .2s,transform .1s}
button:hover{filter:brightness(1.15);transform:translateY(-1px)}
button:disabled{background:#888;cursor:not-allowed;transform:none}
.bd{background:var(--err)}
.bg{background:var(--ok)}
.lg{max-height:140px;overflow-y:auto;background:var(--lt);padding:10px;border-radius:6px;font-family:Consolas,monospace;font-size:.78rem;border:1px solid var(--cbd);margin-top:10px}
.ib{background:linear-gradient(135deg,var(--cb),rgba(52,152,219,.06));border-left:4px solid var(--acc);padding:14px 18px;border-radius:0 8px 8px 0;margin:12px 0}
.wb{background:linear-gradient(135deg,var(--cb),rgba(243,156,18,.08));border-left:4px solid var(--wrn);padding:14px 18px;border-radius:0 8px 8px 0;margin:12px 0}
.g2{display:grid;grid-template-columns:1fr 1fr;gap:14px;margin:14px 0}
.gc{background:var(--cb);border:1px solid var(--cbd);border-radius:10px;padding:16px}
.gc h4{color:var(--hl);margin-bottom:6px}
.iq{background:var(--cb);border:1px solid var(--cbd);border-radius:10px;padding:16px;margin:10px 0;cursor:pointer;transition:box-shadow .2s}
.iq:hover{box-shadow:0 2px 12px rgba(0,0,0,.08)}
.iq h4{color:var(--acc)}
.iq .a{display:none;margin-top:10px;padding-top:10px;border-top:1px dashed var(--cbd)}
.iq.open .a{display:block}
#nb{display:flex;justify-content:space-between;margin-top:auto;padding-top:30px}
@media(max-width:768px){#sidebar{width:220px;min-width:220px}#content{padding:20px}}
</style>
</head>
<body>
<div id="sidebar">
<header>DS Learning Hub</header>
<div class="sc">
<button onclick="toggleTheme()">Dark / Light</button>
<button onclick="show(CH.length-1)">Interview Q&A</button>
</div>
<div id="pb"><div id="pf"></div></div>
<ul id="nl"></ul>
</div>
<div id="content">
<div id="mv"></div>
<div id="nb">
<button onclick="go(-1)">&#8592; Previous</button>
<button onclick="go(1)">Next &#8594;</button>
</div>
</div>
<script>
''')

print("Part 1 ready")
