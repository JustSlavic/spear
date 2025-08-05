#include <corelibs/base.h>

#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

static bool g_vk_physical_device_print = true;
static int width = 800;
static int height = 600;

uint32 get_file_size(char const *filename)
{
    uint32 size = 0;
    int fd = open(filename, O_RDONLY, 0);
    if (fd >= 0)
    {
        struct stat st;
        int ec = fstat(fd, &st);
        if (ec >= 0)
        {
            size = st.st_size;
        }
        close(fd);
    }

    return size;
}

uint32 load_entire_file(char const *filename, uint32 size, void *memory)
{
    uint32 bytes_read = 0;
    int fd = open(filename, O_RDONLY, 0);
    if (fd >= 0)
    {
        bytes_read = read(fd, memory, size);
        close(fd);
    }

    return bytes_read;
}

static char const *vk_physical_device_type_to_string[] =
{
    "VK_PHYSICAL_DEVICE_TYPE_OTHER",
    "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU",
    "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU",
    "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU",
    "VK_PHYSICAL_DEVICE_TYPE_CPU",
};

static char const *vk_validation_layer_names[] =
{
    "VK_LAYER_KHRONOS_validation",
};

char const *vk_result_to_string(VkResult vk_result)
{
    switch (vk_result)
    {
        case VK_SUCCESS: return "VK_SUCCESS";
        case VK_NOT_READY: return "VK_NOT_READY";
        case VK_TIMEOUT: return "VK_TIMEOUT";
        case VK_EVENT_SET: return "VK_EVENT_SET";
        case VK_EVENT_RESET: return "VK_EVENT_RESET";
        case VK_INCOMPLETE: return "VK_INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
        case VK_ERROR_UNKNOWN: return "VK_ERROR_UNKNOWN";
        // case VK_ERROR_VALIDATION_FAILED: return "VK_ERROR_VALIDATION_FAILED";
        case VK_ERROR_OUT_OF_POOL_MEMORY: return "VK_ERROR_OUT_OF_POOL_MEMORY";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
        case VK_ERROR_FRAGMENTATION: return "VK_ERROR_FRAGMENTATION";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
        case VK_PIPELINE_COMPILE_REQUIRED: return "VK_PIPELINE_COMPILE_REQUIRED";
        // case VK_ERROR_NOT_PERMITTED: return "VK_ERROR_NOT_PERMITTED";
        case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
        case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";
        // case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
        case VK_ERROR_INVALID_SHADER_NV: return "VK_ERROR_INVALID_SHADER_NV";
        case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR: return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
        case VK_THREAD_IDLE_KHR: return "VK_THREAD_IDLE_KHR";
        case VK_THREAD_DONE_KHR: return "VK_THREAD_DONE_KHR";
        case VK_OPERATION_DEFERRED_KHR: return "VK_OPERATION_DEFERRED_KHR";
        case VK_OPERATION_NOT_DEFERRED_KHR: return "VK_OPERATION_NOT_DEFERRED_KHR";
        case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT: return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
        case VK_INCOMPATIBLE_SHADER_BINARY_EXT: return "VK_INCOMPATIBLE_SHADER_BINARY_EXT";
        // case VK_PIPELINE_BINARY_MISSING_KHR: return "VK_PIPELINE_BINARY_MISSING_KHR";
        // case VK_ERROR_NOT_ENOUGH_SPACE_KHR: return "VK_ERROR_NOT_ENOUGH_SPACE_KHR";
        default: return "<error>";
    }
    return "<error>";
}

bool is_validation_layers_supported(char const **layer_names, uint layer_count)
{
    uint32_t vk_instance_layer_property_count = 0;
    vkEnumerateInstanceLayerProperties(&vk_instance_layer_property_count, NULL);

    VkLayerProperties *vk_instance_layer_properties = malloc(sizeof(*vk_instance_layer_properties) * vk_instance_layer_property_count);
    vkEnumerateInstanceLayerProperties(&vk_instance_layer_property_count, vk_instance_layer_properties);

    bool all_layers_found = true;
    uint vk_validation_layer_index;
    for (vk_validation_layer_index = 0;
         vk_validation_layer_index < layer_count;
         vk_validation_layer_index++)
    {
        char const *vk_validation_layer = layer_names[vk_validation_layer_index];

        bool found = false;
        uint vk_available_instance_layer_index;
        for (vk_available_instance_layer_index = 0;
             vk_available_instance_layer_index < vk_instance_layer_property_count;
             vk_available_instance_layer_index++)
        {
            VkLayerProperties *vk_available_instance_layer = vk_instance_layer_properties + vk_available_instance_layer_index;
            if (strcmp(vk_validation_layer, vk_available_instance_layer->layerName) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            all_layers_found = false;
            break;
        }
    }

    return all_layers_found;
}

void vk_physical_device_properties_print(VkPhysicalDeviceProperties properties)
{
    printf("Physical Device Properties:\n");
    printf("    deviceName = \"%s\"\n", properties.deviceName);
    printf("    apiVersion = %d.%d.%d\n", VK_API_VERSION_MAJOR(properties.apiVersion), VK_API_VERSION_MINOR(properties.apiVersion), VK_API_VERSION_PATCH(properties.apiVersion));
    printf("    driverVersion = %d.%d.%d\n", VK_API_VERSION_MAJOR(properties.driverVersion), VK_API_VERSION_MINOR(properties.driverVersion), VK_API_VERSION_PATCH(properties.driverVersion));
    printf("    deviceType = %s\n", vk_physical_device_type_to_string[properties.deviceType]);
    printf("    limits (for example: maxImageDimension2D = %d)\n", properties.limits.maxImageDimension2D);
}

void vk_physical_devices_print(VkPhysicalDevice *vk_physical_devices, uint vk_physical_device_count)
{
    uint vk_physical_device_index;
    for (vk_physical_device_index = 0;
         vk_physical_device_index < vk_physical_device_count;
         vk_physical_device_index++)
    {
        VkPhysicalDevice vk_physical_device = vk_physical_devices[vk_physical_device_index];

        VkPhysicalDeviceProperties vk_physical_device_properties;
        vkGetPhysicalDeviceProperties(vk_physical_device, &vk_physical_device_properties);
        vk_physical_device_properties_print(vk_physical_device_properties);

        // VkPhysicalDeviceFeatures vk_physical_device_features;
        // vkGetPhysicalDeviceFeatures(vk_physical_device, &vk_physical_device_features);
        // vk_physical_device_features_print(vk_physical_device_features);
    }
}

int main()
{
    VkResult vk_result;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Vulkan_LoadLibrary(NULL);
    SDL_Window* window = SDL_CreateWindow(
        "Vulkan window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
    if (window == NULL)
    {
        printf("Could not create SDL Window: \"%s\"\n", SDL_GetError());
        exit(3);
    }

    VkApplicationInfo vk_app_info =
    {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Spear engine",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };

    uint32 vk_extension_count = 0;
    uint32 vk_extension_count_additional = 4;
    SDL_Vulkan_GetInstanceExtensions(window, &vk_extension_count, NULL);
    const char **vk_extension_names = malloc(sizeof(*vk_extension_names) * (vk_extension_count + vk_extension_count_additional));
    SDL_Vulkan_GetInstanceExtensions(window, &vk_extension_count, vk_extension_names);

    // This extension is required to create VkInstance for compatibility on MacOS
    vk_extension_names[vk_extension_count++] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
    // This extension is required by the VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    vk_extension_names[vk_extension_count++] = VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME;
    // These extensions are required to present pictures to the screen
    vk_extension_names[vk_extension_count++] = VK_KHR_SURFACE_EXTENSION_NAME;

    printf("vk_extension_count = %d\n", vk_extension_count);
    uint vk_extension_index;
    for (vk_extension_index = 0;
         vk_extension_index < vk_extension_count;
         vk_extension_index++)
    {
        printf("vk_extension_names[%d] = \"%s\";\n", vk_extension_index, vk_extension_names[vk_extension_index]);
    }

    VkInstanceCreateInfo vk_instance_create_info =
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &vk_app_info,
        .enabledExtensionCount = vk_extension_count,
        .ppEnabledExtensionNames = vk_extension_names,
        .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
    };

    if (is_validation_layers_supported(vk_validation_layer_names, ARRAY_COUNT(vk_validation_layer_names)))
    {
        vk_instance_create_info.enabledLayerCount = ARRAY_COUNT(vk_validation_layer_names);
        vk_instance_create_info.ppEnabledLayerNames = vk_validation_layer_names;
    }

    VkInstance vk_instance = {};
    vk_result = vkCreateInstance(&vk_instance_create_info, NULL, &vk_instance);
    if (vk_result != VK_SUCCESS)
    {
        printf("Error: Could not create VkInstance! (%s)\n", vk_result_to_string(vk_result));
        return EXIT_FAILURE;
    }

    VkSurfaceKHR vk_surface;
    if (!SDL_Vulkan_CreateSurface(window, vk_instance, &vk_surface))
    {
        printf("Error: Could not create VkSurfaceKHR!\n");
        return EXIT_FAILURE;
    }

    uint32 vk_physical_device_count = 0;
    vkEnumeratePhysicalDevices(vk_instance, &vk_physical_device_count, NULL);
    VkPhysicalDevice *vk_physical_devices = malloc(sizeof(*vk_physical_devices) * vk_physical_device_count);
    vkEnumeratePhysicalDevices(vk_instance, &vk_physical_device_count, vk_physical_devices);

    VkPhysicalDevice vk_physical_device = vk_physical_devices[0];

    if (g_vk_physical_device_print)
    {
        vk_physical_devices_print(vk_physical_devices, vk_physical_device_count);
    }

    uint32 vk_queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &vk_queue_family_count, NULL);
    VkQueueFamilyProperties *vk_queue_families = malloc(sizeof(*vk_queue_families) * vk_queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &vk_queue_family_count, vk_queue_families);

    uint32 vk_queue_family_index_graphics = UINT32_MAX;
    uint32 vk_queue_family_index_present = UINT32_MAX;

    uint vk_queue_family_index;
    for (vk_queue_family_index = 0;
         vk_queue_family_index < vk_queue_family_count;
         vk_queue_family_index++)
    {
        VkQueueFamilyProperties properties = vk_queue_families[vk_queue_family_index];

        if ((vk_queue_family_index_graphics >= vk_queue_family_count) && (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT))
        {
            vk_queue_family_index_graphics = (int32) vk_queue_family_index;
        }

        if (vk_queue_family_index_present >= vk_queue_family_count)
        {
            printf("vk_queue_family_index = %d\n", vk_queue_family_index);
            VkBool32 is_present_supported = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(vk_physical_device, vk_queue_family_index, vk_surface, &is_present_supported);

            if (is_present_supported)
            {
                vk_queue_family_index_present = (int32) vk_queue_family_index;
            }
        }
    }

    printf("Queue famili indices found: graphics=%d, present=%d\n",
        vk_queue_family_index_graphics, vk_queue_family_index_present);

    // Vulkan does not allow creating two queues of the same family index;
    // @note from vulkan validation layer:
    //     "vkCreateDevice(): pCreateInfo->pQueueCreateInfos[1].queueFamilyIndex (0)
    //     is not unique and was also used in pCreateInfo->pQueueCreateInfos[0].
    //     The Vulkan spec states: The queueFamilyIndex member of each element of
    //     pQueueCreateInfos must be unique within pQueueCreateInfos".
    //
    // For this simplest example I ignore logic of choosing queues.
    ASSERT(vk_queue_family_index_graphics == vk_queue_family_index_present);

    float vk_queue_priorities[] = { 1.0f };
    VkDeviceQueueCreateInfo vk_device_queue_create_info =
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = vk_queue_family_index_graphics,
        .queueCount = 1,
        .pQueuePriorities = vk_queue_priorities,
    };
    VkPhysicalDeviceFeatures vk_physical_device_features = {};
    VkDeviceCreateInfo vk_device_create_info =
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pQueueCreateInfos = &vk_device_queue_create_info,
        .queueCreateInfoCount = 1,
        .pEnabledFeatures = &vk_physical_device_features,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = NULL,
    };

    uint32 vk_device_extension_count = 0;
    vkEnumerateDeviceExtensionProperties(vk_physical_device, NULL, &vk_device_extension_count, NULL);
    VkExtensionProperties *vk_extension_properties = malloc(sizeof(*vk_extension_properties) * vk_device_extension_count);
    vkEnumerateDeviceExtensionProperties(vk_physical_device, NULL, &vk_device_extension_count, vk_extension_properties);

    char const *vk_device_extension_names_used[64] = {};
    uint32 vk_device_extension_count_used = 0;

    bool vk_device_extension_swapchain_supported = false;
    uint vk_device_extension_index;
    for (vk_device_extension_index = 0;
         vk_device_extension_index < vk_device_extension_count;
         vk_device_extension_index++)
    {
        if (strcmp(vk_extension_properties[vk_device_extension_index].extensionName, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME) == 0)
        {
            // Vulkan requires device extension 'VK_KHR_portability_subset', if it supports it.
            // @note from vulkan validation layer:
            //     vkCreateDevice():  VK_KHR_portability_subset must be enabled
            //     because physical device VkPhysicalDevice 0x600000fcc3a0[] supports it.
            //
            // Find the extension in the list of supported extensions, and if it's present there,
            // add it to the list of used device extensions.
            vk_device_extension_names_used[vk_device_extension_count_used++] = VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME;
        }
        if (strcmp(vk_extension_properties[vk_device_extension_index].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
        {
            // This extension is required to present pictures on the screen
            vk_device_extension_names_used[vk_device_extension_count_used++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
            vk_device_extension_swapchain_supported = true;
        }
    }

    printf("vk_device_extension_swapchain_supported = %s\n", PRINT_BOOL(vk_device_extension_swapchain_supported));

    if (vk_device_extension_count_used > 0)
    {
        vk_device_create_info.enabledExtensionCount = vk_device_extension_count_used;
        vk_device_create_info.ppEnabledExtensionNames = vk_device_extension_names_used;
    }

    // =============== Checking the swapchain support =============== //
    VkSurfaceFormatKHR vk_surface_format;
    VkPresentModeKHR vk_present_mode;
    UNUSED(vk_present_mode);

    VkSurfaceCapabilitiesKHR vk_surface_capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physical_device, vk_surface, &vk_surface_capabilities);

    printf("vk_surface_capabilities:\n");
    printf("    minImageCount = %u\n", vk_surface_capabilities.minImageCount);
    printf("    maxImageCount = %u\n", vk_surface_capabilities.maxImageCount);
    printf("    currentExtent = { width = %u, height = %u };\n", vk_surface_capabilities.currentExtent.width, vk_surface_capabilities.currentExtent.height);
    printf("    minImageExtent = { width = %u, height = %u };\n", vk_surface_capabilities.minImageExtent.width, vk_surface_capabilities.minImageExtent.height);
    printf("    maxImageExtent = { width = %u, height = %u };\n", vk_surface_capabilities.maxImageExtent.width, vk_surface_capabilities.maxImageExtent.height);
    printf("    maxImageArrayLayers = %u\n", vk_surface_capabilities.maxImageArrayLayers);
    // VkSurfaceTransformFlagsKHR       supportedTransforms;
    // VkSurfaceTransformFlagBitsKHR    currentTransform;
    // VkCompositeAlphaFlagsKHR         supportedCompositeAlpha;
    // VkImageUsageFlags                supportedUsageFlags;

    uint32 vk_surface_format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physical_device, vk_surface, &vk_surface_format_count, NULL);
    VkSurfaceFormatKHR *vk_surface_formats = malloc(sizeof(*vk_surface_formats) * vk_surface_format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physical_device, vk_surface, &vk_surface_format_count, vk_surface_formats);

    printf("vk_surface_format_count = %d\n", vk_surface_format_count);
    printf("vk_surface_formats:\n");
    uint vk_surface_format_index;
    for (vk_surface_format_index = 0;
         vk_surface_format_index < vk_surface_format_count;
         vk_surface_format_index++)
    {
        if ((vk_surface_formats[vk_surface_format_index].format == VK_FORMAT_B8G8R8A8_SRGB) &&
            (vk_surface_formats[vk_surface_format_index].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
        {
            vk_surface_format = vk_surface_formats[vk_surface_format_index];
        }
    }

    uint32 vk_surface_present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(vk_physical_device, vk_surface, &vk_surface_present_mode_count, NULL);
    VkPresentModeKHR *vk_surface_present_modes = malloc(sizeof(*vk_surface_present_modes) * vk_surface_present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(vk_physical_device, vk_surface, &vk_surface_present_mode_count, vk_surface_present_modes);

    printf("vk_present_modes:\n");
    uint vk_surface_present_mode_index;
    for (vk_surface_present_mode_index = 0;
         vk_surface_present_mode_index < vk_surface_present_mode_count;
         vk_surface_present_mode_index++)
    {
        if (vk_surface_present_modes[vk_surface_present_mode_index] == VK_PRESENT_MODE_FIFO_KHR)
        {
            vk_present_mode = vk_surface_present_modes[vk_surface_present_mode_index];
        }
    }

    VkDevice vk_device;
    vk_result = vkCreateDevice(vk_physical_device, &vk_device_create_info, NULL, &vk_device);
    if (vk_result != VK_SUCCESS)
    {
        printf("Error: Could not create VkDevice (%s)\n", vk_result_to_string(vk_result));
        exit(8);
    }

    VkSwapchainCreateInfoKHR vk_swapchain_create_info =
    {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = vk_surface,
        .minImageCount = vk_surface_capabilities.minImageCount,
        .imageFormat = vk_surface_format.format,
        .imageColorSpace = vk_surface_format.colorSpace,
        .imageExtent = vk_surface_capabilities.currentExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        // Since in this example, the graphics and present queues are the same,
        // use exclusive sharing mode for images in the swapchain.
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = vk_surface_capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = vk_present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE,
    };

    VkSwapchainKHR vk_swapchain;
    vk_result = vkCreateSwapchainKHR(vk_device, &vk_swapchain_create_info, NULL, &vk_swapchain);
    if (vk_result != VK_SUCCESS)
    {
        printf("Could not create swapchain!\n");
        return EXIT_FAILURE;
    }

    uint32 vk_swapchain_image_count = 0;
    vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_swapchain_image_count, NULL);
    VkImage *vk_swapchain_images = malloc(sizeof(*vk_swapchain_images) * vk_swapchain_image_count);
    vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_swapchain_image_count, vk_swapchain_images);

    printf("vk_swapchain_image_count = %d\n", vk_swapchain_image_count);

    uint32 vk_image_view_count = vk_swapchain_image_count;
    VkImageView *vk_image_views = malloc(sizeof(*vk_image_views) * vk_image_view_count);

    uint vk_image_view_index;
    for (vk_image_view_index = 0;
         vk_image_view_index < vk_image_view_count;
         vk_image_view_index++)
    {
        VkImageSubresourceRange vk_subresource_range =
        {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        };
        VkImageViewCreateInfo vk_image_view_create_info =
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = vk_swapchain_images[vk_image_view_index],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = vk_surface_format.format,
            .subresourceRange = vk_subresource_range,
        };
        vk_result = vkCreateImageView(vk_device, &vk_image_view_create_info, NULL, vk_image_views + vk_image_view_index);
        if (vk_result != VK_SUCCESS)
        {
            printf("Error: Could not create VkImageView\n");
            exit(1);
        }
    }

    VkQueue vk_queue;
    vkGetDeviceQueue(vk_device, vk_queue_family_index_graphics, 0, &vk_queue);

    uint32 vk_vertex_shader_size = get_file_size("example.vs.spv");
    if (vk_vertex_shader_size == 0)
    {
        printf("Error: Could not find file \"example.vs.spv\"");
    }
    void *vk_vertex_shader_bytecode = malloc(vk_vertex_shader_size);
    uint32 bytes_read = load_entire_file("example.vs.spv", vk_vertex_shader_size, vk_vertex_shader_bytecode);
    if (bytes_read != vk_vertex_shader_size)
    {
        printf("Error: trying to read shader \"example.vs.spv\"; "
               "Filesize is %d bytes, read %d bytes\n", vk_vertex_shader_size, bytes_read);
        return EXIT_FAILURE;
    }
    printf("bytes_read = %d (alignment=%d)\n", bytes_read, get_alignment(vk_vertex_shader_bytecode));

    uint32 vk_fragment_shader_size = get_file_size("example.fs.spv");
    if (vk_fragment_shader_size == 0)
    {
        printf("Error: Could not find file \"example.fs.spv\"");
    }
    void *vk_fragment_shader_bytecode = malloc(vk_fragment_shader_size);
    bytes_read = load_entire_file("example.fs.spv", vk_fragment_shader_size, vk_fragment_shader_bytecode);
    if (bytes_read != vk_fragment_shader_size)
    {
        printf("Error: trying to read shader \"example.fs.spv\"; "
               "Filesize is %d bytes, read %d bytes\n", vk_fragment_shader_size, bytes_read);
        return EXIT_FAILURE;
    }
    printf("bytes_read = %d (alignment=%d)\n", bytes_read, get_alignment(vk_fragment_shader_bytecode));

    VkShaderModule vk_shader_module_vert;
    {
        VkShaderModuleCreateInfo vk_shader_module_create_info =
        {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = vk_vertex_shader_size,
            .pCode = vk_vertex_shader_bytecode,
        };
        vk_result = vkCreateShaderModule(vk_device, &vk_shader_module_create_info, NULL, &vk_shader_module_vert);
        if (vk_result != VK_SUCCESS)
        {
            printf("Error: Could not create shader module for vertex shader!\n");
            return EXIT_FAILURE;
        }
    }
    VkShaderModule vk_shader_module_frag;
    {
        VkShaderModuleCreateInfo vk_shader_module_create_info =
        {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = vk_fragment_shader_size,
            .pCode = vk_fragment_shader_bytecode,
        };
        vk_result = vkCreateShaderModule(vk_device, &vk_shader_module_create_info, NULL, &vk_shader_module_frag);
        if (vk_result != VK_SUCCESS)
        {
            printf("Error: Could not create shader module for fragment shader!\n");
            return EXIT_FAILURE;
        }
    }

    VkPipelineShaderStageCreateInfo vk_pipeline_shader_stage_create_info_vert =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vk_shader_module_vert,
        .pName = "main",
    };
    UNUSED(vk_pipeline_shader_stage_create_info_vert);

    VkPipelineShaderStageCreateInfo vk_pipeline_shader_stage_create_info_frag =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = vk_shader_module_frag,
        .pName = "main",
    };
    UNUSED(vk_pipeline_shader_stage_create_info_frag);

    uint32 vk_dynamic_state_count = 2;
    VkDynamicState vk_dynamic_states[] =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    VkPipelineDynamicStateCreateInfo vk_pipeline_dynamic_state_create_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = vk_dynamic_state_count,
        .pDynamicStates = vk_dynamic_states,
    };
    UNUSED(vk_pipeline_dynamic_state_create_info);

    VkPipelineVertexInputStateCreateInfo vk_pipeline_vertex_input_state_create_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 0,
        .pVertexBindingDescriptions = NULL, // Optional
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions = NULL, // Optional
    };
    UNUSED(vk_pipeline_vertex_input_state_create_info);

    VkPipelineInputAssemblyStateCreateInfo vk_pipeline_input_assembly_state_create_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };
    UNUSED(vk_pipeline_input_assembly_state_create_info);

    VkViewport vk_viewport =
    {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float) vk_surface_capabilities.currentExtent.width,
        .height = (float) vk_surface_capabilities.currentExtent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };
    UNUSED(vk_viewport);

    VkRect2D vk_scissor =
    {
        .offset = {0, 0},
        .extent = vk_surface_capabilities.currentExtent,
    };
    UNUSED(vk_scissor);

    VkPipelineViewportStateCreateInfo vk_pipeline_viewport_state_create_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = &vk_viewport,
        .scissorCount = 1,
        .pScissors = &vk_scissor,
    };
    UNUSED(vk_pipeline_viewport_state_create_info);


    VkPipelineRasterizationStateCreateInfo vk_pipeline_rasterization_state_create_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .lineWidth = 1.0f,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
    };
    UNUSED(vk_pipeline_rasterization_state_create_info);

    VkPipelineMultisampleStateCreateInfo vk_pipeline_multisample_state_create_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .sampleShadingEnable = VK_FALSE,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    };
    UNUSED(vk_pipeline_multisample_state_create_info);


    VkPipelineColorBlendAttachmentState vk_pipeline_color_blend_attachment_state =
    {
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        .blendEnable = VK_FALSE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
        .colorBlendOp = VK_BLEND_OP_ADD, // Optional
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
        .alphaBlendOp = VK_BLEND_OP_ADD, // Optional
    };
    UNUSED(vk_pipeline_color_blend_attachment_state);


    VkPipelineLayout vk_pipeline_layout;
    VkPipelineLayoutCreateInfo vk_pipeline_layout_create_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0, // Optional
        .pSetLayouts = NULL, // Optional
        .pushConstantRangeCount = 0, // Optional
        .pPushConstantRanges = NULL, // Optional
    };
    vkCreatePipelineLayout(vk_device, &vk_pipeline_layout_create_info, NULL, &vk_pipeline_layout);




    for (vk_image_view_index = 0;
         vk_image_view_index < vk_image_view_count;
         vk_image_view_index++)
    {
        vkDestroyImageView(vk_device, vk_image_views[vk_image_view_index], NULL);
    }

    vkDestroyPipelineLayout(vk_device, vk_pipeline_layout, NULL);
    vkDestroyShaderModule(vk_device, vk_shader_module_vert, NULL);
    vkDestroyShaderModule(vk_device, vk_shader_module_frag, NULL);
    vkDestroySwapchainKHR(vk_device, vk_swapchain, NULL);
    vkDestroyDevice(vk_device, NULL);
    vkDestroySurfaceKHR(vk_instance, vk_surface, NULL);
    vkDestroyInstance(vk_instance, NULL);

    return 0;
}

#include <corelibs/base.c>








