#include "gas.h"

#ifdef EVM_GAS
void init_gas(evm_t *evm) {
    // prepare evm gas
    evm->refund = 0;
    if (!evm->init_gas) evm->init_gas = evm->gas;
}

void update_account_code(evm_t *evm, account_t* new_account) {
    // prepare evm gas
    if (new_account)
        new_account->code = evm->return_data;
}

 void evm_init(evm_t *evm) {
    evm->accounts = NULL;
    evm->gas = 0;
    evm->logs = NULL;
    evm->parent = NULL;
    evm->refund = 0;
    evm->init_gas = 0;
}

 void finalize_and_refund_gas(evm_t *evm) {
    uint64_t gas_used = evm->init_gas - evm->gas;
    if ((evm->properties & EVM_PROP_NO_FINALIZE) == 0) {
        // finalize and refund
        if (evm->refund && evm->parent) {
            evm->parent->gas -= gas_used;
            evm->gas += gas_used + evm->refund;
        } else {
            evm->gas += min(evm->refund, gas_used >> 1);
        }
    }
}

 void finalize_subcall_gas(evm_t* evm, int success, evm_t* parent) {
    // if it was successfull we copy the new state to the parent
    if (success == 0 && evm->state != EVM_STATE_REVERTED)
        copy_state(parent, evm);
    // if we have gas left and it was successfull we returen it to the parent process.
    if (success == 0) parent->gas += evm->gas;

}


int selfdestruct_gas(evm_t *evm) {
    uint8_t adr[20], l, *p;
    if (evm_stack_pop(evm, adr, 20) < 0) return EVM_ERROR_EMPTY_STACK;
    account_t *self_account = evm_get_account(evm, evm->address, 1);
// TODO check if this account was selfsdesstructed before
    evm->refund += R_SELFDESTRUCT;

    l = 32;
    p = self_account->balance;
    optimize_len(p, l);
    if (l && (l > 1 || *p != 0)) {
        if (evm_get_account(evm, adr, 0) == NULL) {
            if ((evm->properties & EVM_PROP_NO_FINALIZE) == 0) subgas(G_NEWACCOUNT);
            evm_get_account(evm, adr, 1);
        }
        if (transfer_value(evm, evm->address, adr, self_account->balance, 32, 0) < 0) return EVM_ERROR_OUT_OF_GAS;
    }
    memset(self_account->balance, 0, 32);
    memset(self_account->nonce, 0, 32);
    self_account->code.len = 0;
    storage_t *s = NULL;
    while (self_account->storage) {
        s = self_account->storage;
        self_account->storage = s->next;
        _free(s);
    }
    evm->state = EVM_STATE_STOPPED;
    return 0;
}

#endif