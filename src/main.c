#include <efi/efi.h>
#include <efi/efilib.h>
#include <stdbool.h>

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  InitializeLib(ImageHandle, SystemTable);
  Print(L"Hello, world!\n");

  Print(L"Die\n");
  while(true);
  return EFI_SUCCESS;
}

