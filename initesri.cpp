
#import "C:\Program Files (x86)\ArcGIS\Desktop10.1\com\esriSystem.olb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "VARTYPE","XMLSerializer")

#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" no_namespace raw_interfaces_only no_implementation rename("esriProductCode", "esriVersionProductCode")

#define PRODUCTSTRING(ver) L"" L#ver

#define ESRI_SET_VERSION(prod,vers){									\
    HRESULT hr;															\
    VARIANT_BOOL vb;													\
    _bstr_t bsVer(PRODUCTSTRING(vers));									\
    IArcGISVersionPtr ipVersion(__uuidof(VersionManager));				\
    if (!SUCCEEDED(hr = ipVersion->LoadVersion(prod, bsVer, &vb)))		\
        fprintf(stderr, "LoadVersion() failed with code 0x%.8x\n", hr);	\
        else if (vb != VARIANT_TRUE)										\
        fprintf(stderr, "LoadVersion() failed\n");						\
}


VARIANT_BOOL InitializeLicense(esriLicenseProductCode product) {
    IAoInitializePtr ipInit(CLSID_AoInitialize);
    esriLicenseStatus licenseStatus = esriLicenseFailure;
    ipInit->IsProductCodeAvailable(product, &licenseStatus);
    if (licenseStatus == esriLicenseAvailable)
        ipInit->Initialize(product, &licenseStatus);
    return (licenseStatus == esriLicenseCheckedOut);
}


void InitESRI()
{
    CoInitialize(nullptr);

    ESRI_SET_VERSION(esriArcGISDesktop, 10.1)

    InitializeLicense(esriLicenseProductCodeAdvanced);
}
