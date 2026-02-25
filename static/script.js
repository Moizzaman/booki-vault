async function checkInit() {
    const res = await fetch('/action', { method: 'POST', body: new URLSearchParams({'cmd': 'check_init'}) });
    const data = await res.json();
    document.getElementById(data.output === "NEED_SETUP" ? 'setupDiv' : 'loginDiv').style.display = 'block';
}

function show(p) {
    document.querySelectorAll('.panel').forEach(el => el.style.display = 'none');
    document.getElementById(p + 'P').style.display = 'block';
}

async function terminateApp() {
    if(confirm("Exit and lock the vault?")) {
        document.body.innerHTML = "<div style='text-align:center; margin-top:20%; font-family:sans-serif;'><h1>Vault Locked.</h1><p>Server stopped.</p></div>";
        fetch('/shutdown', {method: 'POST'});
    }
}

async function run(type) {
    const btnText = document.getElementById('btnText');
    const spinner = document.getElementById('spinner');
    
    if (type === 'add') { 
        btnText.innerText = "Encrypting..."; 
        spinner.style.display = "inline-block"; 
    }

    let params = new URLSearchParams({'cmd': type});
    if (type === 'setup') params.append('v1', document.getElementById('setupPass').value);
    if (type === 'auth') params.append('v1', document.getElementById('loginPass').value);
    if (type === 'add') { 
        params.append('v1', document.getElementById('aS').value); 
        params.append('v2', document.getElementById('aP').value); 
    }
    if (type === 'change') params.append('v1', document.getElementById('newM').value);

    const res = await fetch('/action', { method: 'POST', body: params });
    const data = await res.json();

    if (data.output === "CHANGED" || data.output === "SETUP_SUCCESS" || data.output === "CLEARED") {
        location.reload();
    } else if (data.output === "SUCCESS") {
        document.getElementById('loginDiv').style.display = 'none';
        document.getElementById('mainDiv').style.display = 'block';
    } else if (type === 'view') {
        const items = data.output.split('|');
        document.getElementById('list').innerHTML = items.map(i => i ? `<div class='item'>${i}</div>` : '').join('');
    } else if (type === 'add') {
        setTimeout(() => {
            spinner.style.display = "none";
            btnText.innerText = "Save Securely";
            alert("Saved to vault!");
        }, 800);
    } else {
        alert(data.output);
    }
}

window.onload = checkInit;