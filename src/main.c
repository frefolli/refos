#include <efi/efi.h>
#include <efi/efilib.h>
#include <stdbool.h>

void loader_println(const wchar_t *msg) {
  Print(L"EFI Loader :: \"%s\"\n\r", msg);
}

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  // Initialize EFI::lib
  InitializeLib(ImageHandle, SystemTable);
  loader_println(L"Starting Session");

  // Locating GOP protocol data (for VESA usage)
  EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
  EFI_STATUS Status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
  if(EFI_ERROR(Status))
    loader_println(L"Unable to locate GOP");
  else
    loader_println(L"GOP has been located");

  // Hanging
  loader_println(L"Ending Session");
  while(true);
  return EFI_SUCCESS;
}
