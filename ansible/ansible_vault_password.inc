vault_pass_file="${HOME}/.handshake_vault_password"
if ! test -f ${vault_pass_file}
    then
        dd if=/dev/random bs=1 count=24 2>/dev/null | base64 > ${vault_pass_file}
    fi
